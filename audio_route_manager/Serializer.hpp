/*
 * Copyright (C) 2017 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "RouteManagerConfig.hpp"
#include <stdint.h>
#include <string>
#include <utils/Errors.h>

struct _xmlNode;
struct _xmlDoc;

namespace audio_hal
{

struct AudioCriterionTypeTraits
{
    static const char *const tag;
    static const char *const collectionTag;

    struct Attributes
    {
        static const char name[];
        static const char type[];
        static const char values[];
    };

    typedef CriterionType Element;
    typedef CriterionType *PtrElement;
    typedef CriterionTypes Collection;
    typedef RouteManagerConfig *PtrSerializingCtx;

    static android::status_t deserialize(_xmlDoc *doc, const _xmlNode *root, PtrElement &element,
                                         PtrSerializingCtx serializingContext);
};

struct AudioCriterionTraits
{
    static const char *const tag;
    static const char *const collectionTag;

    struct Attributes
    {
        static const char name[];
        static const char type[];
        static const char defaultVal[];
        static const char parameter[];
        static const char mapping[];
    };

    typedef Criterion Element;
    typedef Criterion *PtrElement;
    typedef Criteria Collection;
    typedef RouteManagerConfig *PtrSerializingCtx;

    static android::status_t deserialize(_xmlDoc *doc, const _xmlNode *root, PtrElement &element,
                                         PtrSerializingCtx serializingContext);
};

struct RogueParameterTraits
{
    static const char *const tag;
    static const char *const collectionTag;

    struct Attributes
    {
        static const char name[];
        static const char type[];
        static const char mapping[];
        static const char defaultVal[];
        static const char parameter[];
        static const char path[];
    };

    typedef Parameter Element;
    typedef Parameter *PtrElement;
    typedef Parameters Collection;
    typedef RouteManagerConfig *PtrSerializingCtx;

    static android::status_t deserialize(_xmlDoc *doc, const _xmlNode *root, PtrElement &element,
                                         PtrSerializingCtx serializingContext);
};

struct AudioProfileTraits
{
    static const char *const tag;
    static const char *const collectionTag;

    struct Attributes
    {
        static const char samplingRates[];
        static const char format[];
        static const char channelMasks[];
    };

    typedef AudioCapability Element;
    typedef AudioCapability PtrElement;
    typedef AudioCapabilities Collection;
    typedef void *PtrSerializingCtx;

    static android::status_t deserialize(_xmlDoc *doc, const _xmlNode *root, PtrElement &element,
                                         PtrSerializingCtx serializingContext);
};

struct DevicePortTraits
{
    static const char *const tag;
    static const char *const collectionTag;

    struct Attributes
    {
        static const char name[];
        static const char type[];
        static const char role[];
        static const char address[];
    };

    struct DevicePort
    {
        DevicePort(audio_devices_t type, const std::string &name, const std::string &address) :
            mType(type), mName(name), mAddress(address) {}

        audio_devices_t mType;
        std::string mName;
        std::string mAddress;
    };
    struct DevicePorts : public std::vector<DevicePort *>
    {
        DevicePort *findByName(const std::string &name) const
        {
            for (auto port : *this) {
                if (port->mName == name) {
                    return port;
                }
            }
            return nullptr;
        }
    };
    typedef DevicePort Element;
    typedef DevicePort *PtrElement;
    typedef DevicePorts Collection;
    typedef void *PtrSerializingCtx;

    static android::status_t deserialize(_xmlDoc *doc, const _xmlNode *root, PtrElement &element,
                                         PtrSerializingCtx serializingContext);
};

struct RouteTraits
{
    static const char *const tag;
    static const char *const collectionTag;

    struct Attributes
    {
        static const char sink[];
        static const char sources[];
    };

    struct Route
    {
        Route(const std::string &sinkName, const std::vector<std::string> &sources) :
            mSink(sinkName), mSources(sources) {}

        bool involveSource(const std::string &name)
        {
            for (const auto &source : mSources)
            {
                if (source == name)
                    return true;
            }
            return false;
        }

        std::string mSink;
        std::vector<std::string> mSources;
    };
    typedef Route Element;
    typedef Route *PtrElement;
    typedef std::vector<Route *> Collection;
    typedef void *PtrSerializingCtx;

    static android::status_t deserialize(_xmlDoc *doc, const _xmlNode *root, PtrElement &element,
                                         PtrSerializingCtx serializingContext);
};

struct ModuleTraits
{
    static const char *const tag;
    static const char *const collectionTag;

    struct Module {};
    struct Context
    {
        DevicePortTraits::Collection mDevicePorts;
        RouteTraits::Collection mRoutes;
    };
    typedef Module Element;
    typedef Module *PtrElement;
    typedef std::vector<Module *> Collection;
    typedef RouteManagerConfig *PtrSerializingCtx;

    static android::status_t deserialize(_xmlDoc *doc, const _xmlNode *root, PtrElement &element,
                                         PtrSerializingCtx /*serializingContext*/);
};

struct MixPortTraits
{
    static const char *const tag;
    static const char *const collectionTag;

    struct Attributes
    {
        static const char name[];
        static const char role[];
        static const char card[];
        static const char device[];
        static const char deviceAddress[];
        static const char flagMask[];
        static const char requirePreEnable[];
        static const char requirePostDisable[];
        static const char silencePrologMs[];
        static const char channelsPolicy[];
        static const char channelPolicyCopy[];
        static const char channelPolicyIgnore[];
        static const char channelPolicyAverage[];
        static const char periodSize[];
        static const char periodCount[];
        static const char startThreshold[];
        static const char stopThreshold[];
        static const char silenceThreshold[];
        static const char availMin[];
        static const char dynamicChannelMapsControl[];
        static const char dynamicSampleRatesControl[];
        static const char dynamicFormatsControl[];
        static const char supportedUseCases[];
        static const char devicePorts[];
        static const char effects[];
    };

    typedef AudioStreamRoute Element;
    typedef AudioStreamRoute *PtrElement;
    typedef StreamRouteCollection Collection;
    typedef ModuleTraits::Context *PtrSerializingCtx;

    static android::status_t deserialize(_xmlDoc *doc, const _xmlNode *root, PtrElement &element,
                                         PtrSerializingCtx serializingContext);

    // MixPort has no child
};

class RouteSerializer
{
private:
    static const char *const rootName;

    static const char *const versionAttribute;
    static const uint32_t gMajor; /**< the major number of the policy xml format version. */
    static const uint32_t gMinor; /**< the minor number of the policy xml format version. */
    typedef RouteManagerConfig Element;

public:
    RouteSerializer();
    android::status_t deserialize(const char *str, RouteManagerConfig &config);

private:
    std::string mRootElementName;
    std::string mVersion;
};

}  // namespace audio_hal
