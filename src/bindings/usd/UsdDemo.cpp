/*
 * A deliberately small, prototype-grade surface for demonstrating that the
 * OpenUSD integration works from the browser: supply USD documents at runtime,
 * let USD compose them, and resolve assets through Ar.
 *
 * Everything here is a free function taking and returning primitives, so there
 * is no object lifetime for JS to manage and no disposal pattern to learn. No
 * stage state is held between calls either - anything that depends on payload
 * load state takes it as a parameter instead. That costs a re-open per call,
 * which is cheap because Sdf caches the underlying layers.
 *
 * This is not a proposed shape for the real USD API. It exists to make the
 * integration observable from a test.
 */

#include "emscripten/bind.h"

#include <filesystem>
#include <fstream>
#include <limits>
#include <mutex>
#include <string>
#include <vector>

#include <pxr/pxr.h>

#include <pxr/base/plug/registry.h>
#include <pxr/usd/ar/asset.h>
#include <pxr/usd/ar/resolvedPath.h>
#include <pxr/usd/ar/resolver.h>
#include <pxr/usd/sdf/layer.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/sphere.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace
{

/*
 * USD locates its schemas through the plugInfo.json tree that CSP embeds in the
 * virtual filesystem. Natively it derives that search path from the location of
 * its own shared libraries; under wasm there is none, and PXR_PLUGINPATH_NAME is
 * only consulted while PlugRegistry initialises - so a setenv() here would be
 * racing static initialisation. Registering explicitly on first use has no such
 * ordering requirement.
 */
void EnsureUsdInitialised()
{
    static std::once_flag Once;
    std::call_once(Once, [] { PlugRegistry::GetInstance().RegisterPlugins(CSP_USD_PLUGIN_PATH); });
}

UsdStageRefPtr OpenStage(const std::string& DocumentPath, bool LoadPayloads)
{
    EnsureUsdInitialised();

    return UsdStage::Open(DocumentPath, LoadPayloads ? UsdStage::LoadAll : UsdStage::LoadNone);
}

/* Newline-joined rather than a bound vector, to keep the surface primitive-only. */
std::string Join(const std::vector<std::string>& Values)
{
    std::string Joined;

    for (const std::string& Value : Values)
    {
        if (!Joined.empty())
        {
            Joined += '\n';
        }

        Joined += Value;
    }

    return Joined;
}

} // namespace

/*
 * Places a USD document in the virtual filesystem so documents fetched or
 * generated on the JS side can be composed. Relative asset paths between
 * documents resolve against wherever they are written, which is what makes the
 * cross-document test meaningful.
 */
bool WriteUsdDocument(const std::string& Path, const std::string& Contents)
{
    const std::filesystem::path Target(Path);

    if (Target.has_parent_path())
    {
        std::error_code Error;
        std::filesystem::create_directories(Target.parent_path(), Error);
    }

    std::ofstream Out(Path, std::ios::binary | std::ios::trunc);

    if (!Out)
    {
        return false;
    }

    Out << Contents;

    return Out.good();
}

/* Empty if the document could not be opened at all. */
std::string UsdDefaultPrimPath(const std::string& DocumentPath)
{
    const UsdStageRefPtr Stage = OpenStage(DocumentPath, true);

    if (!Stage)
    {
        return {};
    }

    const UsdPrim Default = Stage->GetDefaultPrim();

    return Default ? Default.GetPath().GetString() : std::string {};
}

/*
 * Paths of every composed prim, newline separated. Uses USD's default traversal
 * predicate, so an unloaded payload's contents are genuinely absent rather than
 * merely empty - which is how the payload half of the composition test tells
 * deferred loading apart from a plain reference.
 */
std::string UsdPrimPaths(const std::string& DocumentPath, bool LoadPayloads)
{
    const UsdStageRefPtr Stage = OpenStage(DocumentPath, LoadPayloads);

    if (!Stage)
    {
        return {};
    }

    std::vector<std::string> Paths;

    for (const UsdPrim& Prim : Stage->Traverse())
    {
        Paths.push_back(Prim.GetPath().GetString());
    }

    return Join(Paths);
}

/* NaN when the prim is missing or is not a sphere, so JS can tell "no value" apart from 0. */
double UsdSphereRadius(const std::string& DocumentPath, const std::string& PrimPath)
{
    const UsdStageRefPtr Stage = OpenStage(DocumentPath, true);

    if (!Stage)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    const UsdGeomSphere Sphere(Stage->GetPrimAtPath(SdfPath(PrimPath)));

    if (!Sphere)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double Radius = 0.0;

    if (!Sphere.GetRadiusAttr().Get(&Radius))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return Radius;
}

/*
 * Identifiers of every layer that contributed to the composed stage. This is the
 * direct evidence that composition really did pull in the other documents,
 * rather than the values happening to look right.
 */
std::string UsdContributingDocuments(const std::string& DocumentPath)
{
    const UsdStageRefPtr Stage = OpenStage(DocumentPath, true);

    if (!Stage)
    {
        return {};
    }

    std::vector<std::string> Identifiers;

    for (const SdfLayerHandle& Layer : Stage->GetUsedLayers())
    {
        if (Layer)
        {
            Identifiers.push_back(Layer->GetIdentifier());
        }
    }

    return Join(Identifiers);
}

/*
 * Ar 2.0 separates working out what an asset path means in context
 * (CreateIdentifier) from working out where it actually lives (Resolve). The
 * three functions below expose that split as-is.
 */
std::string ArCreateIdentifier(const std::string& AssetPath, const std::string& AnchorPath)
{
    EnsureUsdInitialised();

    return ArGetResolver().CreateIdentifier(AssetPath, ArResolvedPath(AnchorPath));
}

/* Empty when nothing backs the path, rather than throwing. */
std::string ArResolve(const std::string& AssetPath)
{
    EnsureUsdInitialised();

    return ArGetResolver().Resolve(AssetPath).GetPathString();
}

/*
 * Opens a resolved asset and returns its leading bytes. Reading through Ar is
 * how Sdf's file format plugins get at content, so this exercises the path that
 * actually matters rather than just checking a filename came back.
 */
std::string ArReadAssetHead(const std::string& ResolvedPath, int ByteCount)
{
    EnsureUsdInitialised();

    const std::shared_ptr<ArAsset> Asset = ArGetResolver().OpenAsset(ArResolvedPath(ResolvedPath));

    if (!Asset || ByteCount <= 0)
    {
        return {};
    }

    const std::shared_ptr<const char> Buffer = Asset->GetBuffer();

    if (!Buffer)
    {
        return {};
    }

    const size_t Available = Asset->GetSize();
    const size_t Wanted = static_cast<size_t>(ByteCount);

    return std::string(Buffer.get(), Available < Wanted ? Available : Wanted);
}

EMSCRIPTEN_BINDINGS(UsdDemoModule)
{
    emscripten::function("WriteUsdDocument", &WriteUsdDocument);
    emscripten::function("UsdDefaultPrimPath", &UsdDefaultPrimPath);
    emscripten::function("UsdPrimPaths", &UsdPrimPaths);
    emscripten::function("UsdSphereRadius", &UsdSphereRadius);
    emscripten::function("UsdContributingDocuments", &UsdContributingDocuments);
    emscripten::function("ArCreateIdentifier", &ArCreateIdentifier);
    emscripten::function("ArResolve", &ArResolve);
    emscripten::function("ArReadAssetHead", &ArReadAssetHead);
}
