/***
*
 *	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/

#pragma once

#include "CPointEntity.h"

constexpr int MAX_MULTI_TARGETS = 64; // maximum number of targets a single multi_manager entity may be assigned.

#define SF_GROUP_DEBUG 2

class CInfoGroup : public CPointEntity
{
    DECLARE_CLASS(CInfoGroup, CPointEntity);
    DECLARE_DATAMAP();
    
public:
    void Spawn() override;
    bool KeyValue(KeyValueData* pkvd) override;
    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
    string_t GetMember(const char* szMemberName);
    
    int m_cMembers;
    string_t m_iszMemberName[MAX_MULTI_TARGETS];
    string_t m_iszMemberValue[MAX_MULTI_TARGETS];
    string_t m_iszDefaultMember;
};