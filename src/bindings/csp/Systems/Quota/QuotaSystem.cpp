#include "../../../async/Promises.h"
#include "../../../containers/Array.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Quota/Quota.h"
#include "CSP/Systems/Quota/QuotaSystem.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::QuotaSystem>(csp::systems::QuotaSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPQuotaSystem)
{
    emscripten::class_<csp::systems::QuotaSystem, emscripten::base<csp::systems::SystemBase>>("QuotaSystem")
        .function(
            "getTotalSpacesOwnedByUser",
            +[](csp::systems::QuotaSystem& self) {
                return Promisify<PromiseOfFeatureLimitResult>([&](emscripten::val cb) { self.GetTotalSpacesOwnedByUser(ToNativeCallback(cb.as<FeatureLimitCallback>())); });
            })
        .function(
            "getConcurrentUsersInSpace(spaceId)",
            +[](csp::systems::QuotaSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfFeatureLimitResult>(
                    [&](emscripten::val cb) { self.GetConcurrentUsersInSpace(spaceId, ToNativeCallback(cb.as<FeatureLimitCallback>())); });
            })
        .function(
            "getTotalSpaceSizeInKilobytes(spaceId)",
            +[](csp::systems::QuotaSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfFeatureLimitResult>(
                    [&](emscripten::val cb) { self.GetTotalSpaceSizeInKilobytes(spaceId, ToNativeCallback(cb.as<FeatureLimitCallback>())); });
            })
        .function(
            "getTierFeatureProgressForUser(featureNames)",
            +[](csp::systems::QuotaSystem& self, const csp::common::Array<csp::systems::TierFeatures>& featureNames) {
                return Promisify<PromiseOfFeaturesLimitResult>(
                    [&](emscripten::val cb) { self.GetTierFeatureProgressForUser(featureNames, ToNativeCallback(cb.as<FeaturesLimitCallback>())); });
            })
        .function(
            "getTierFeatureProgressForSpace(spaceId, featureNames)",
            +[](csp::systems::QuotaSystem& self, const csp::common::String& spaceId, const csp::common::Array<csp::systems::TierFeatures>& featureNames) {
                return Promisify<PromiseOfFeaturesLimitResult>(
                    [&](emscripten::val cb) { self.GetTierFeatureProgressForSpace(spaceId, featureNames, ToNativeCallback(cb.as<FeaturesLimitCallback>())); });
            })
        .function(
            "setUserTier(tier, userId)",
            +[](csp::systems::QuotaSystem& self, csp::systems::TierNames tier, const csp::common::String& userId) {
                return Promisify<PromiseOfUserTierResult>([&](emscripten::val cb) { self.SetUserTier(tier, userId, ToNativeCallback(cb.as<UserTierCallback>())); });
            })
        .function(
            "getCurrentUserTier",
            +[](csp::systems::QuotaSystem& self) {
                return Promisify<PromiseOfUserTierResult>([&](emscripten::val cb) { self.GetCurrentUserTier(ToNativeCallback(cb.as<UserTierCallback>())); });
            })
        .function(
            "getTierFeatureQuota(tierName, featureName)",
            +[](csp::systems::QuotaSystem& self, csp::systems::TierNames tierName, csp::systems::TierFeatures featureName) {
                return Promisify<PromiseOfFeatureQuotaResult>(
                    [&](emscripten::val cb) { self.GetTierFeatureQuota(tierName, featureName, ToNativeCallback(cb.as<FeatureQuotaCallback>())); });
            })
        .function(
            "getTierFeaturesQuota(tierName)", +[](csp::systems::QuotaSystem& self, csp::systems::TierNames tierName) {
                return Promisify<PromiseOfFeaturesQuotaResult>([&](emscripten::val cb) { self.GetTierFeaturesQuota(tierName, ToNativeCallback(cb.as<FeaturesQuotaCallback>())); });
            });
}
