// Copyright 2018 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __SECURITY_ACCESSCONTROL_GOVERNANCEPARSER_H__
#define __SECURITY_ACCESSCONTROL_GOVERNANCEPARSER_H__

#include "CommonParser.h"

namespace eprosima {
namespace fastrtps {
namespace rtps {
namespace security {

enum class ProtectionKind
{
    NONE,
    SIGN,
    ENCRYPT
};

struct DomainRule
{
    Domains domains;
    ProtectionKind rtps_protection_kind;
};

struct DomainAccessRules
{
    std::vector<DomainRule> rules;
};

class GovernanceParser
{
    public:

        bool parse_stream(const char* stream, size_t stream_length);

        void swap(DomainAccessRules& rules);

    private:

        bool parse_domain_access_rules_node(tinyxml2::XMLElement* root);

        bool parse_domain_access_rules(tinyxml2::XMLElement* root);

        bool parse_domain_rule(tinyxml2::XMLElement* root, DomainRule& rule);

        DomainAccessRules access_rules_;
};

}
}
}
}

#endif // __SECURITY_ACCESSCONTROL_GOVERNANCEPARSER_H__