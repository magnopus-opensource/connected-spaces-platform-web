#include "../../../containers/Array.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/ECommerce/ECommerce.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"
#include <utility>

EMSCRIPTEN_BINDINGS(CSPECommerce)
{
    emscripten::class_<csp::systems::CurrencyInfo>("CurrencyInfo")
        .class_function(
            "create", +[]() { return csp::systems::CurrencyInfo(); })
        .property("amount", &csp::systems::CurrencyInfo::Amount)
        .property("currencyCode", &csp::systems::CurrencyInfo::CurrencyCode)
        .function("equals(other)", &csp::systems::CurrencyInfo::operator==);

    emscripten::class_<csp::systems::ProductMediaInfo>("ProductMediaInfo")
        .class_function(
            "create", +[]() { return csp::systems::ProductMediaInfo(); })
        .property("mediaContentType", &csp::systems::ProductMediaInfo::MediaContentType)
        .property("alt", &csp::systems::ProductMediaInfo::Alt)
        .property("url", &csp::systems::ProductMediaInfo::Url)
        .property("width", &csp::systems::ProductMediaInfo::Width)
        .property("height", &csp::systems::ProductMediaInfo::Height)
        .function("equals(other)", &csp::systems::ProductMediaInfo::operator==);

    emscripten::class_<csp::systems::VariantOptionInfo>("VariantOptionInfo")
        .class_function(
            "create", +[]() { return csp::systems::VariantOptionInfo(); })
        .property("name", &csp::systems::VariantOptionInfo::Name)
        .property("value", &csp::systems::VariantOptionInfo::Value)
        .function("equals(other)", &csp::systems::VariantOptionInfo::operator==);

    emscripten::class_<csp::systems::ProductVariantInfo>("ProductVariantInfo")
        .class_function(
            "create", +[]() { return csp::systems::ProductVariantInfo(); })
        .property("id", &csp::systems::ProductVariantInfo::Id)
        .property("title", &csp::systems::ProductVariantInfo::Title)
        .property("url", &csp::systems::ProductVariantInfo::Url)
        .property("availableForSale", &csp::systems::ProductVariantInfo::AvailableForSale)
        .property("availableStock", &csp::systems::ProductVariantInfo::AvailableStock)
        .function(
            "getMedia", +[](const csp::systems::ProductVariantInfo& self) { return self.Media; })
        .function(
            "setMedia(value)", +[](csp::systems::ProductVariantInfo& self, csp::systems::ProductMediaInfo value) { self.Media = std::move(value); })
        .function(
            "getOptions",
            +[](const csp::systems::ProductVariantInfo& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::VariantOptionInfo>> { self.Options }; })
        .function(
            "setOptions(value)", +[](csp::systems::ProductVariantInfo& self, csp::common::Array<csp::systems::VariantOptionInfo> value) { self.Options = std::move(value); })
        .function(
            "getUnitPrice", +[](const csp::systems::ProductVariantInfo& self) { return self.UnitPrice; })
        .function(
            "setUnitPrice(value)", +[](csp::systems::ProductVariantInfo& self, csp::systems::CurrencyInfo value) { self.UnitPrice = std::move(value); })
        .function("equals(other)", &csp::systems::ProductVariantInfo::operator==);

    emscripten::class_<csp::systems::ProductInfo>("ProductInfo")
        .class_function(
            "create", +[]() { return csp::systems::ProductInfo(); })
        .property("id", &csp::systems::ProductInfo::Id)
        .property("title", &csp::systems::ProductInfo::Title)
        .property("createdAt", &csp::systems::ProductInfo::CreatedAt)
        .property("description", &csp::systems::ProductInfo::Description)
        .property("tags", &csp::systems::ProductInfo::Tags)
        .function(
            "getVariants",
            +[](const csp::systems::ProductInfo& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::ProductVariantInfo>> { self.Variants }; })
        .function(
            "setVariants(value)", +[](csp::systems::ProductInfo& self, csp::common::Array<csp::systems::ProductVariantInfo> value) { self.Variants = std::move(value); })
        .function(
            "getMedia", +[](const csp::systems::ProductInfo& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::ProductMediaInfo>> { self.Media }; })
        .function(
            "setMedia(value)", +[](csp::systems::ProductInfo& self, csp::common::Array<csp::systems::ProductMediaInfo> value) { self.Media = std::move(value); })
        .function("equals(other)", &csp::systems::ProductInfo::operator==);

    emscripten::class_<csp::systems::CheckoutInfo>("CheckoutInfo")
        .class_function(
            "create", +[]() { return csp::systems::CheckoutInfo(); })
        .property("storeUrl", &csp::systems::CheckoutInfo::StoreUrl)
        .property("checkoutUrl", &csp::systems::CheckoutInfo::CheckoutUrl)
        .function("equals(other)", &csp::systems::CheckoutInfo::operator==);

    emscripten::class_<csp::systems::CartLine>("CartLine")
        .class_function(
            "create", +[]() { return csp::systems::CartLine(); })
        .property("cartLineId", &csp::systems::CartLine::CartLineId)
        .property("productVariantId", &csp::systems::CartLine::ProductVariantId)
        .property("quantity", &csp::systems::CartLine::Quantity)
        .function("equals(other)", &csp::systems::CartLine::operator==);

    emscripten::class_<csp::systems::CartInfo>("CartInfo")
        .class_function(
            "create", +[]() { return csp::systems::CartInfo(); })
        .property("spaceId", &csp::systems::CartInfo::SpaceId)
        .property("cartId", &csp::systems::CartInfo::CartId)
        .property("totalQuantity", &csp::systems::CartInfo::TotalQuantity)
        .function(
            "getCartLines", +[](const csp::systems::CartInfo& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::CartLine>> { self.CartLines }; })
        .function(
            "setCartLines(value)", +[](csp::systems::CartInfo& self, csp::common::Array<csp::systems::CartLine> value) { self.CartLines = std::move(value); })
        .function("equals(other)", &csp::systems::CartInfo::operator==);

    emscripten::class_<csp::systems::ShopifyStoreInfo>("ShopifyStoreInfo")
        .class_function(
            "create", +[]() { return csp::systems::ShopifyStoreInfo(); })
        .property("storeId", &csp::systems::ShopifyStoreInfo::StoreId)
        .property("storeName", &csp::systems::ShopifyStoreInfo::StoreName)
        .property("spaceOwnerId", &csp::systems::ShopifyStoreInfo::SpaceOwnerId)
        .property("spaceId", &csp::systems::ShopifyStoreInfo::SpaceId)
        .property("isEcommerceActive", &csp::systems::ShopifyStoreInfo::IsEcommerceActive)
        .function("equals(other)", &csp::systems::ShopifyStoreInfo::operator==);

    emscripten::class_<csp::systems::ProductInfoResult, emscripten::base<csp::systems::ResultBase>>("ProductInfoResult")
        .function(
            "getProductInfo", +[](const csp::systems::ProductInfoResult& self) { return self.GetProductInfo(); });

    emscripten::class_<csp::systems::ProductInfoCollectionResult, emscripten::base<csp::systems::ResultBase>>("ProductInfoCollectionResult")
        .function(
            "getProducts", +[](const csp::systems::ProductInfoCollectionResult& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::systems::ProductInfo>> { self.GetProducts() };
            });

    emscripten::class_<csp::systems::CheckoutInfoResult, emscripten::base<csp::systems::ResultBase>>("CheckoutInfoResult")
        .function(
            "getCheckoutInfo", +[](const csp::systems::CheckoutInfoResult& self) { return self.GetCheckoutInfo(); });

    emscripten::class_<csp::systems::CartInfoResult, emscripten::base<csp::systems::ResultBase>>("CartInfoResult")
        .function(
            "getCartInfo", +[](const csp::systems::CartInfoResult& self) { return self.GetCartInfo(); });

    emscripten::class_<csp::systems::AddShopifyStoreResult, emscripten::base<csp::systems::ResultBase>>("AddShopifyStoreResult")
        .function(
            "getShopifyStoreInfo", +[](const csp::systems::AddShopifyStoreResult& self) { return self.GetShopifyStoreInfo(); });

    emscripten::class_<csp::systems::GetShopifyStoresResult, emscripten::base<csp::systems::ResultBase>>("GetShopifyStoresResult")
        .function(
            "getShopifyStores", +[](const csp::systems::GetShopifyStoresResult& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::systems::ShopifyStoreInfo>> { self.GetShopifyStores() };
            });

    emscripten::class_<csp::systems::ValidateShopifyStoreResult, emscripten::base<csp::systems::ResultBase>>("ValidateShopifyStoreResult")
        .property("validateResult", &csp::systems::ValidateShopifyStoreResult::ValidateResult);
}
