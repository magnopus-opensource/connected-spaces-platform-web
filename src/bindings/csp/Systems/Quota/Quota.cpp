#include "../../../containers/Array.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Quota/Quota.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPQuota)
{
    emscripten::enum_<csp::systems::PeriodEnum>("PeriodEnum", emscripten::enum_value_type::number)
        .value("Total", csp::systems::PeriodEnum::Total)
        .value("CalendarMonth", csp::systems::PeriodEnum::CalendarMonth)
        .value("Hours24", csp::systems::PeriodEnum::Hours24)
        .value("Invalid", csp::systems::PeriodEnum::Invalid);

    emscripten::enum_<csp::systems::TierFeatures>("TierFeatures", emscripten::enum_value_type::number)
        .value("SpaceOwner", csp::systems::TierFeatures::SpaceOwner)
        .value("ScopeConcurrentUsers", csp::systems::TierFeatures::ScopeConcurrentUsers)
        .value("ObjectCaptureUpload", csp::systems::TierFeatures::ObjectCaptureUpload)
        .value("AudioVideoUpload", csp::systems::TierFeatures::AudioVideoUpload)
        .value("TotalUploadSizeInKilobytes", csp::systems::TierFeatures::TotalUploadSizeInKilobytes)
        .value("Agora", csp::systems::TierFeatures::Agora)
        .value("OpenAI", csp::systems::TierFeatures::OpenAI)
        .value("GoogleGenAI", csp::systems::TierFeatures::GoogleGenAI)
        .value("Shopify", csp::systems::TierFeatures::Shopify)
        .value("TicketedSpace", csp::systems::TierFeatures::TicketedSpace)
        .value("Invalid", csp::systems::TierFeatures::Invalid);

    emscripten::enum_<csp::systems::TierNames>("TierNames", emscripten::enum_value_type::number)
        .value("Basic", csp::systems::TierNames::Basic)
        .value("Premium", csp::systems::TierNames::Premium)
        .value("Pro", csp::systems::TierNames::Pro)
        .value("Enterprise", csp::systems::TierNames::Enterprise)
        .value("Invalid", csp::systems::TierNames::Invalid);

    emscripten::function(
        "tierNameEnumToString(value)", +[](const csp::systems::TierNames& value) { return csp::common::String { csp::systems::TierNameEnumToString(value) }; });
    emscripten::function(
        "tierFeatureEnumToString(value)", +[](const csp::systems::TierFeatures& value) { return csp::common::String { csp::systems::TierFeatureEnumToString(value) }; });
    emscripten::function("stringToTierNameEnum(value)", &csp::systems::StringToTierNameEnum);
    emscripten::function("stringToTierFeatureEnum(value)", &csp::systems::StringToTierFeatureEnum);

    emscripten::class_<csp::systems::FeatureLimitInfo>("FeatureLimitInfo")
        .class_function(
            "create", +[]() { return csp::systems::FeatureLimitInfo(); })
        .property("featureName", &csp::systems::FeatureLimitInfo::FeatureName)
        .property("activityCount", &csp::systems::FeatureLimitInfo::ActivityCount)
        .property("limit", &csp::systems::FeatureLimitInfo::Limit)
        .function("equals(other)", &csp::systems::FeatureLimitInfo::operator==);

    emscripten::class_<csp::systems::UserTierInfo>("UserTierInfo")
        .class_function(
            "create", +[]() { return csp::systems::UserTierInfo(); })
        .property("assignToType", &csp::systems::UserTierInfo::AssignToType)
        .property("assignToId", &csp::systems::UserTierInfo::AssignToId)
        .property("tierName", &csp::systems::UserTierInfo::TierName)
        .function("equals(other)", &csp::systems::UserTierInfo::operator==);

    emscripten::class_<csp::systems::FeatureQuotaInfo>("FeatureQuotaInfo")
        .class_function(
            "create", +[]() { return csp::systems::FeatureQuotaInfo(); })
        .property("featureName", &csp::systems::FeatureQuotaInfo::FeatureName)
        .property("tierName", &csp::systems::FeatureQuotaInfo::TierName)
        .property("limit", &csp::systems::FeatureQuotaInfo::Limit)
        .property("period", &csp::systems::FeatureQuotaInfo::Period)
        .function("equals(other)", &csp::systems::FeatureQuotaInfo::operator==);

    emscripten::class_<csp::systems::FeaturesLimitResult, emscripten::base<csp::systems::ResultBase>>("FeaturesLimitResult")
        .function(
            "getFeaturesLimitInfo", +[](const csp::systems::FeaturesLimitResult& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::systems::FeatureLimitInfo>> { self.GetFeaturesLimitInfo() };
            });

    emscripten::class_<csp::systems::FeatureLimitResult, emscripten::base<csp::systems::ResultBase>>("FeatureLimitResult")
        .function(
            "getFeatureLimitInfo", +[](const csp::systems::FeatureLimitResult& self) { return self.GetFeatureLimitInfo(); });

    emscripten::class_<csp::systems::UserTierResult, emscripten::base<csp::systems::ResultBase>>("UserTierResult")
        .function(
            "getUserTierInfo", +[](const csp::systems::UserTierResult& self) { return self.GetUserTierInfo(); });

    emscripten::class_<csp::systems::FeatureQuotaResult, emscripten::base<csp::systems::ResultBase>>("FeatureQuotaResult")
        .function(
            "getFeatureQuotaInfo", +[](const csp::systems::FeatureQuotaResult& self) { return self.GetFeatureQuotaInfo(); });

    emscripten::class_<csp::systems::FeaturesQuotaResult, emscripten::base<csp::systems::ResultBase>>("FeaturesQuotaResult")
        .function(
            "getFeaturesQuotaInfo", +[](const csp::systems::FeaturesQuotaResult& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::systems::FeatureQuotaInfo>> { self.GetFeaturesQuotaInfo() };
            });
}
