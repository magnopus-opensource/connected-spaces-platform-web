#include "../../../async/Promises.h"
#include "../../../containers/Array.h"
#include "../../../containers/Optional.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/ECommerce/ECommerce.h"
#include "CSP/Systems/ECommerce/ECommerceSystem.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::ECommerceSystem>(csp::systems::ECommerceSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPECommerceSystem)
{
    emscripten::class_<csp::systems::ECommerceSystem, emscripten::base<csp::systems::SystemBase>>("ECommerceSystem")
        .function(
            "getProductInformation(spaceId, productId)",
            +[](csp::systems::ECommerceSystem& self, const csp::common::String& spaceId, const csp::common::String& productId) {
                return Promisify<PromiseOfProductInfoResult>(
                    [&](emscripten::val cb) { self.GetProductInformation(spaceId, productId, ToNativeCallback(cb.as<ProductInfoResultCallback>())); });
            })
        .function(
            "getProductInfoCollectionByVariantIds(spaceId, variantIds)",
            +[](csp::systems::ECommerceSystem& self, const csp::common::String& spaceId, const csp::common::Array<csp::common::String>& variantIds) {
                return Promisify<PromiseOfProductInfoCollectionResult>(
                    [&](emscripten::val cb) { self.GetProductInfoCollectionByVariantIds(spaceId, variantIds, ToNativeCallback(cb.as<ProductInfoCollectionResultCallback>())); });
            })
        .function(
            "getCheckoutInformation(spaceId, cartId)",
            +[](csp::systems::ECommerceSystem& self, const csp::common::String& spaceId, const csp::common::String& cartId) {
                return Promisify<PromiseOfCheckoutInfoResult>(
                    [&](emscripten::val cb) { self.GetCheckoutInformation(spaceId, cartId, ToNativeCallback(cb.as<CheckoutInfoResultCallback>())); });
            })
        .function(
            "createCart(spaceId)",
            +[](csp::systems::ECommerceSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfCartInfoResult>([&](emscripten::val cb) { self.CreateCart(spaceId, ToNativeCallback(cb.as<CartInfoResultCallback>())); });
            })
        .function(
            "getCart(spaceId, cartId)",
            +[](csp::systems::ECommerceSystem& self, const csp::common::String& spaceId, const csp::common::String& cartId) {
                return Promisify<PromiseOfCartInfoResult>([&](emscripten::val cb) { self.GetCart(spaceId, cartId, ToNativeCallback(cb.as<CartInfoResultCallback>())); });
            })
        .function(
            "getShopifyStores(isActive)",
            +[](csp::systems::ECommerceSystem& self, const csp::common::Optional<bool>& isActive) {
                return Promisify<PromiseOfGetShopifyStoresResult>(
                    [&](emscripten::val cb) { self.GetShopifyStores(isActive, ToNativeCallback(cb.as<GetShopifyStoresResultCallback>())); });
            })
        .function(
            "addShopifyStore(storeName, spaceId, isEcommerceActive, privateAccessToken)",
            +[](csp::systems::ECommerceSystem& self, const csp::common::String& storeName, const csp::common::String& spaceId, bool isEcommerceActive,
                 const csp::common::String& privateAccessToken) {
                return Promisify<PromiseOfAddShopifyStoreResult>([&](emscripten::val cb) {
                    self.AddShopifyStore(storeName, spaceId, isEcommerceActive, privateAccessToken, ToNativeCallback(cb.as<AddShopifyStoreResultCallback>()));
                });
            })
        .function(
            "setECommerceActiveInSpace(storeName, spaceId, isEcommerceActive)",
            +[](csp::systems::ECommerceSystem& self, const csp::common::String& storeName, const csp::common::String& spaceId, bool isEcommerceActive) {
                return Promisify<PromiseOfAddShopifyStoreResult>(
                    [&](emscripten::val cb) { self.SetECommerceActiveInSpace(storeName, spaceId, isEcommerceActive, ToNativeCallback(cb.as<AddShopifyStoreResultCallback>())); });
            })
        .function(
            "validateShopifyStore(storeName, privateAccessToken)",
            +[](csp::systems::ECommerceSystem& self, const csp::common::String& storeName, const csp::common::String& privateAccessToken) {
                return Promisify<PromiseOfValidateShopifyStoreResult>(
                    [&](emscripten::val cb) { self.ValidateShopifyStore(storeName, privateAccessToken, ToNativeCallback(cb.as<ValidateShopifyStoreResultCallback>())); });
            })
        .function(
            "updateCartInformation(cartInformation)", +[](csp::systems::ECommerceSystem& self, const csp::systems::CartInfo& cartInformation) {
                return Promisify<PromiseOfCartInfoResult>(
                    [&](emscripten::val cb) { self.UpdateCartInformation(cartInformation, ToNativeCallback(cb.as<CartInfoResultCallback>())); });
            });
}
