// Copyright (C) 2018-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <typeindex>
#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <gtest/gtest.h>
#include <ngraph/node.hpp>
#include <ngraph/function.hpp>
#include <ie_plugin_config.hpp>
#include <ngraph/function.hpp>
#include <ngraph_functions/subgraph_builders.hpp>
#include "gtest/gtest.h"
#include "common_test_utils/common_utils.hpp"
#include "common_test_utils/test_common.hpp"

#include "functional_test_utils/skip_tests_config.hpp"
#include "functional_test_utils/plugin_cache.hpp"
#include "functional_test_utils/blob_utils.hpp"
#include "functional_test_utils/precision_utils.hpp"

#include "ngraph_functions/utils/ngraph_helpers.hpp"
#include "ngraph_functions/pass/convert_prc.hpp"


namespace BehaviorTestsUtils {
    typedef std::tuple<
            InferenceEngine::Precision,         // Network precision
            std::string,                        // Device name
            std::map<std::string, std::string>  // Config
    > BehaviorParams;

class BehaviorTestsCommon : public testing::WithParamInterface<BehaviorParams>,
                            public CommonTestUtils::TestsCommon {
public:
    virtual std::string getTestCaseName(testing::TestParamInfo<BehaviorParams> obj);
    void SetUp() override;
    void TearDown() override;
protected:
    BehaviorTestsCommon();

    ~BehaviorTestsCommon() override;

    std::shared_ptr<ngraph::Function> function;
    InferenceEngine::Precision netPrecision;
    std::string targetDevice;
    std::map<std::string, std::string> configuration;
};

    std::string BehaviorTestsCommon::getTestCaseName(testing::TestParamInfo<BehaviorParams> obj){
        std::tie(netPrecision, targetDevice, configuration) = obj.param;
        std::ostringstream result;
        result << "netPRC=" << netPrecision.name() << "_";
        result << "targetDevice=" << targetDevice;
        if (!configuration.empty()) {
            for (auto& configItem : configuration) {
                result << "configItem=" << configItem.first << "_" << configItem.second << "_";
            }
        }
        return result.str();
    }

    void BehaviorTestsCommon::SetUp() {
        std::tie(netPrecision, targetDevice, configuration) = this->GetParam();
        function = ngraph::builder::subgraph::makeConvPoolRelu();
    }

    void BehaviorTestsCommon::TearDown() {
        if (targetDevice.find(CommonTestUtils::DEVICE_GPU) != std::string::npos) {
            PluginCache::get().reset();
        }
    }


}  // namespace LayerTestsUtils