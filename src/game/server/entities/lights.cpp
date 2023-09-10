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
/**
 *	@file
 *	spawn and think functions for editor-placed lights
 */

#include "cbase.h"

string_t GetStdLightStyle(int iStyle)
{
	switch (iStyle)
	{
	// 0 normal
	case 0:
		return MAKE_STRING("m");

	// 1 FLICKER (first variety)
	case 1:
		return MAKE_STRING("mmnmmommommnonmmonqnmmo");

	// 2 SLOW STRONG PULSE
	case 2:
		return MAKE_STRING("abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba");

	// 3 CANDLE (first variety)
	case 3:
		return MAKE_STRING("mmmmmaaaaammmmmaaaaaabcdefgabcdefg");

	// 4 FAST STROBE
	case 4:
		return MAKE_STRING("mamamamamama");

	// 5 GENTLE PULSE 1
	case 5:
		return MAKE_STRING("jklmnopqrstuvwxyzyxwvutsrqponmlkj");

	// 6 FLICKER (second variety)
	case 6:
		return MAKE_STRING("nmonqnmomnmomomno");

	// 7 CANDLE (second variety)
	case 7:
		return MAKE_STRING("mmmaaaabcdefgmmmmaaaammmaamm");

	// 8 CANDLE (third variety)
	case 8:
		return MAKE_STRING("mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa");

	// 9 SLOW STROBE (fourth variety)
	case 9:
		return MAKE_STRING("aaaaaaaazzzzzzzz");

	// 10 FLUORESCENT FLICKER
	case 10:
		return MAKE_STRING("mmamammmmammamamaaamammma");

	// 11 SLOW PULSE NOT FADE TO BLACK
	case 11:
		return MAKE_STRING("abcdefghijklmnopqrrqponmlkjihgfedcba");

	// 12 UNDERWATER LIGHT MUTATION
	// this light only distorts the lightmap - no contribution
	// is made to the brightness of affected surfaces
	case 12:
		return MAKE_STRING("mmnnmmnnnmmnn");

	// 13 OFF
	case 13:
		return MAKE_STRING("a");

	// 14 SLOW FADE IN
	case 14:
		return MAKE_STRING("aabbccddeeffgghhiijjkkllmmmmmmmmmmmmmm");

	// 15 MED FADE IN
	case 15:
		return MAKE_STRING("abcdefghijklmmmmmmmmmmmmmmmmmmmmmmmmmm");

	// 16 FAST FADE IN
	case 16:
		return MAKE_STRING("acegikmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");

	// 17 SLOW FADE OUT
	case 17:
		return MAKE_STRING("llkkjjiihhggffeeddccbbaaaaaaaaaaaaaaaa");

	// 18 MED FADE OUT
	case 18:
		return MAKE_STRING("lkjihgfedcbaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	// 19 FAST FADE OUT
	case 19:
		return MAKE_STRING("kigecaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	default:
		return MAKE_STRING("m");
	}
}

class CLight : public CPointEntity
{
	DECLARE_CLASS(CLight, CPointEntity);
	DECLARE_DATAMAP();

public:
	bool KeyValue(KeyValueData* pkvd) override;
	void Spawn() override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
    void Think() override;
    STATE GetState() { return m_iState; }
    
    string_t GetStyle() { return m_iszCurrentStyle; }
    void SetStyle(string_t iszPattern);

    void SetCorrectStyle();
private:
    STATE m_iState = STATE_OFF;	// current state
    int m_iOnStyle = 0;        // style to use while on
    int m_iOffStyle = 0;       // style to use while off
    int m_iTurnOnStyle = 0;    // style to use while turning on
    int m_iTurnOffStyle = 0;   // style to use while turning off
    int m_iTurnOnTime = 0;     // time taken to turn on
    int m_iTurnOffTime = 0;    // time taken to turn off
    string_t m_iszCurrentStyle; // current style string
	string_t m_iszPattern;      // pattern string
};

LINK_ENTITY_TO_CLASS(light, CLight);

BEGIN_DATAMAP(CLight)
    DEFINE_FIELD(m_iszPattern, FIELD_STRING),
    DEFINE_FIELD(m_iszCurrentStyle, FIELD_STRING),
    DEFINE_FIELD(m_iOnStyle, FIELD_INTEGER),
    DEFINE_FIELD(m_iOffStyle, FIELD_INTEGER),
    DEFINE_FIELD(m_iTurnOnStyle, FIELD_INTEGER),
    DEFINE_FIELD(m_iTurnOffStyle, FIELD_INTEGER),
    DEFINE_FIELD(m_iTurnOnTime, FIELD_INTEGER),
    DEFINE_FIELD(m_iTurnOffTime, FIELD_INTEGER),
END_DATAMAP();

bool CLight::KeyValue(KeyValueData* pkvd)
{
    if (FStrEq(pkvd->szKeyName, "m_iOnStyle"))
    {
        m_iOnStyle = atoi(pkvd->szValue);
        return true;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iOffStyle"))
    {
        m_iOffStyle = atoi(pkvd->szValue);
        return true;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iTurnOnStyle"))
    {
        m_iTurnOnStyle = atoi(pkvd->szValue);
        return true;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iTurnOffStyle"))
    {
        m_iTurnOffStyle = atoi(pkvd->szValue);
        return true;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iTurnOnTime"))
    {
        m_iTurnOnTime = atoi(pkvd->szValue);
        return true;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iTurnOffTime"))
    {
        m_iTurnOffTime = atoi(pkvd->szValue);
        return true;
    }
	else if (FStrEq(pkvd->szKeyName, "pitch"))
	{
		pev->angles.x = atof(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "pattern"))
	{
		m_iszPattern = ALLOC_STRING(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "firetarget"))
	{
	    pev->target = ALLOC_STRING(pkvd->szValue);
	    return true;
	}

	return CPointEntity::KeyValue(pkvd);
}

void CLight::SetStyle(string_t iszPattern)
{
    if (m_iStyle < 32) // if it's using a global style, don't change it
        return;
    
    m_iszCurrentStyle = iszPattern;
    
    //	ALERT(at_console, "SetStyle %d \"%s\"\n", m_iStyle, (char *)STRING( iszPattern ));
    LIGHT_STYLE(m_iStyle, (char*)STRING(iszPattern));
}

// regardless of what's been set by trigger_lightstyle ents, set the style I think I need
void CLight::SetCorrectStyle()
{
    if (m_iStyle >= 32)
    {
        switch(m_iState)
        {
        case STATE_ON:
            if (!FStringNull(m_iszPattern)) // custom styles have priority over standard ones
                SetStyle(m_iszPattern);
            else if (m_iOnStyle)
                SetStyle(GetStdLightStyle(m_iOnStyle));
            else
                SetStyle(MAKE_STRING("m"));
            break;
        case STATE_OFF:
            if (m_iOffStyle)
                SetStyle(GetStdLightStyle(m_iOffStyle));
            else
                SetStyle(MAKE_STRING("a"));
            break;
        case STATE_TURN_ON:
            if (m_iTurnOnStyle)
                SetStyle(GetStdLightStyle(m_iTurnOnStyle));
            else
                SetStyle(MAKE_STRING("a"));
            break;
        case STATE_TURN_OFF:
            if (m_iTurnOffStyle)
                SetStyle(GetStdLightStyle(m_iTurnOffStyle));
            else
                SetStyle(MAKE_STRING("m"));
            break;
        }
    }
    else
        m_iszCurrentStyle = GetStdLightStyle(m_iStyle);
}

void CLight::Spawn()
{
	if (FStringNull(pev->targetname))
	{ // inert light
		REMOVE_ENTITY(edict());
		return;
	}

    if (FBitSet(pev->spawnflags, SF_LIGHT_START_OFF))
        m_iState = STATE_OFF;
    else
        m_iState = STATE_ON;
	
    SetCorrectStyle();
}

void CLight::Think()
{
    switch(GetState())
    {
    case STATE_TURN_ON:
        m_iState = STATE_ON;
        FireTargets(STRING(pev->target), this, this, USE_ON, 0);
        break;
    case STATE_TURN_OFF:
        m_iState = STATE_OFF;
        FireTargets(STRING(pev->target), this, this, USE_OFF, 0);
        break;
    default: // shouldn't happen
        break;
    }
    
    SetCorrectStyle();
}

void CLight::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	if (m_iStyle >= 32)
	{
		if (!ShouldToggle(useType, !FBitSet(pev->spawnflags, SF_LIGHT_START_OFF)))
			return;

	    switch(GetState())
	    {
	    case STATE_ON:
        case STATE_TURN_ON:
            if (m_iTurnOffTime)
            {
                m_iState = STATE_TURN_OFF;
                SetNextThink(m_iTurnOffTime);
            }
            else
            {
                SetBits(pev->spawnflags, SF_LIGHT_START_OFF);
                m_iState = STATE_OFF;
            }
	        break;
	    case STATE_OFF:
        case STATE_TURN_OFF:
            if (m_iTurnOnTime)
            {
                m_iState = STATE_TURN_ON;
                SetNextThink(m_iTurnOnTime);
            }
            else
            {
                m_iState = STATE_ON;
                ClearBits(pev->spawnflags, SF_LIGHT_START_OFF);
            }
	        break;
        default: break;
        }
	    
	    SetCorrectStyle();
	}
}

/**
 *	@brief shut up spawn functions for new spotlights
 */
LINK_ENTITY_TO_CLASS(light_spot, CLight);

class CEnvLight : public CLight
{
public:
	bool KeyValue(KeyValueData* pkvd) override;
	void Spawn() override;
};

LINK_ENTITY_TO_CLASS(light_environment, CEnvLight);

bool CEnvLight::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "_light"))
	{
		int r = 0, g = 0, b = 0, v = 0;
		char szColor[64];
		const int j = sscanf(pkvd->szValue, "%d %d %d %d\n", &r, &g, &b, &v);
		if (j == 1)
		{
			g = b = r;
		}
		else if (j == 4)
		{
			r = r * (v / 255.0);
			g = g * (v / 255.0);
			b = b * (v / 255.0);
		}

		// simulate qrad direct, ambient,and gamma adjustments, as well as engine scaling
		r = pow(r / 114.0, 0.6) * 264;
		g = pow(g / 114.0, 0.6) * 264;
		b = pow(b / 114.0, 0.6) * 264;

		sprintf(szColor, "%d", r);
		CVAR_SET_STRING("sv_skycolor_r", szColor);
		sprintf(szColor, "%d", g);
		CVAR_SET_STRING("sv_skycolor_g", szColor);
		sprintf(szColor, "%d", b);
		CVAR_SET_STRING("sv_skycolor_b", szColor);

		return true;
	}

	return CLight::KeyValue(pkvd);
}

void CEnvLight::Spawn()
{
	char szVector[64];
	UTIL_MakeAimVectors(pev->angles);

	sprintf(szVector, "%f", gpGlobals->v_forward.x);
	CVAR_SET_STRING("sv_skyvec_x", szVector);
	sprintf(szVector, "%f", gpGlobals->v_forward.y);
	CVAR_SET_STRING("sv_skyvec_y", szVector);
	sprintf(szVector, "%f", gpGlobals->v_forward.z);
	CVAR_SET_STRING("sv_skyvec_z", szVector);

	CLight::Spawn();
}
