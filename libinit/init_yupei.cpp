/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <string>

using android::base::GetProperty;

namespace {

constexpr const char* kRoPropsSources[] = {
    "",
    "bootimage.",
    "odm.",
    "product.",
    "system.",
    "system_dlkm.",
    "system_ext.",
    "vendor.",
    "vendor_dlkm.",
};

void property_override(const char* prop, const char* value, bool add = true) {
    auto* pi = const_cast<prop_info*>(__system_property_find(prop));
    if (pi != nullptr) {
        __system_property_update(pi, value, strlen(value));
    } else if (add) {
        __system_property_add(prop, strlen(prop), value, strlen(value));
    }
}

void set_ro_build_prop(const std::string& prop, const std::string& value) {
    for (const auto* source : kRoPropsSources) {
        auto prop_name = std::string("ro.") + source + "build." + prop;
        property_override(prop_name.c_str(), value.c_str(), false);
    }
}

void set_ro_product_prop(const std::string& prop, const std::string& value) {
    for (const auto* source : kRoPropsSources) {
        auto prop_name = std::string("ro.product.") + source + prop;
        property_override(prop_name.c_str(), value.c_str(), false);
    }
}

struct VariantInfo {
    const char* model;
    const char* name;
    const char* marketname;
    const char* mod_device;
    const char* build_fingerprint;
    const char* build_desc;
};

constexpr VariantInfo kYupeiCn = {
    .model = "25097RP43C",
    .name = "yupei",
    .marketname = "Xiaomi Pad 8",
    .mod_device = "yupei_global",
    .build_fingerprint = "Xiaomi/yupei/yupei:15/AQ3A.250226.002/OS4.0.11.0.XPZCN:user/release-keys",
    .build_desc = "yupei-user 15 AQ3A.250226.002 OS4.0.11.0.XPZCN release-keys",
};

constexpr VariantInfo kYupeiIn = {
    .model = "25097RP43I",
    .name = "yupei_in",
    .marketname = "Xiaomi Pad 8",
    .mod_device = "yupei_in_global",
    .build_fingerprint = "Xiaomi/yupei_in/yupei:15/AQ3A.250226.002/OS4.0.11.0.XPZCN:user/release-keys",
    .build_desc = "yupei_in-user 15 AQ3A.250226.002 OS4.0.11.0.XPZCN release-keys",
};

constexpr VariantInfo kYupeiGlobal = {
    .model = "25097RP43G",
    .name = "yupei_global",
    .marketname = "Xiaomi Pad 8",
    .mod_device = "yupei_global",
    .build_fingerprint = "Xiaomi/yupei_global/yupei:15/AQ3A.250226.002/OS3.0.303.0.WPZINXM:user/release-keys",
    .build_desc = "yupei_global-user 15 AQ3A.250226.002 OS3.0.303.0.WPZINXM release-keys",
};

constexpr VariantInfo kYupeidCn = {
    .model = "M656BA",
    .name = "yupei_d",
    .marketname = "Xiaomi Pad 9",
    .mod_device = "yupei_d_global",
    .build_fingerprint = "Xiaomi/yupei_d/yupei:15/AQ3A.250226.002/OS4.0.11.0.XPZCN:user/release-keys",
    .build_desc = "yupei_d-user 15 AQ3A.250226.002 OS4.0.11.0.XPZCN release-keys",
};

constexpr VariantInfo kYupeidGlobal = {
    .model = "26103RP65G",
    .name = "yupei_d_global",
    .marketname = "Xiaomi Pad 9",
    .mod_device = "yupei_d_global",
    .build_fingerprint = "Xiaomi/yupei_d/yupei:15/AQ3A.250226.002/OS4.0.11.0.XPZCN:user/release-keys",
    .build_desc = "yupei_d-user 15 AQ3A.250226.002 OS4.0.11.0.XPZCN release-keys",
};

void apply_variant(const VariantInfo& v) {
    set_ro_product_prop("model", v.model);
    set_ro_product_prop("name", v.name);
    set_ro_product_prop("marketname", v.marketname);
    property_override("ro.product.marketname", v.marketname);
    property_override("ro.product.mod_device", v.mod_device);
    property_override("ro.product.model_for_attestation", v.model);
    property_override("ro.product.name_for_attestation", v.name);
    set_ro_build_prop("fingerprint", v.build_fingerprint);
    property_override("ro.build.description", v.build_desc);
}

}  // namespace

void vendor_load_properties() {
    const std::string sku = GetProperty("ro.boot.hardware.sku", "");
    const std::string hwc = GetProperty("ro.boot.hwc", "");

    if (sku == "yupeid") {
        if (hwc == "CN" || hwc.empty()) {
            apply_variant(kYupeidCn);
        } else {
            apply_variant(kYupeidGlobal);
        }
    } else {
        if (hwc == "CN") {
            apply_variant(kYupeiCn);
        } else if (hwc == "IN" || hwc == "INDIA") {
            apply_variant(kYupeiIn);
        } else {
            apply_variant(kYupeiGlobal);
        }
    }
}
