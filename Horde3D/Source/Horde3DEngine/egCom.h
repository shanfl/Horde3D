// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2016 Nicolas Schulz and Horde3D team
//
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/legal/epl-v10.html
//
// *************************************************************************************************

#ifndef _egCom_H_
#define _egCom_H_

#include "egPrerequisites.h"
#include <string>
#include <queue>
#include <cstdarg>
#include "utTimer.h"


namespace Horde3D {

class GPUTimer;

// =================================================================================================
// Engine Config
// =================================================================================================

struct EngineOptions
{
	enum List
	{
		MaxLogLevel = 1,
		MaxNumMessages,
		TrilinearFiltering,
		MaxAnisotropy,
		TexCompression,
		SRGBLinearization,
		LoadTextures,
		FastAnimation,
		ShadowMapSize,
		SampleCount,
		WireframeMode,
		DebugViewMode,
		DumpFailedShaders,
		GatherTimeStats,
		DumpShaderFile
	};
};

// =================================================================================================

class EngineConfig
{
public:
	EngineConfig();

	float getOption( EngineOptions::List param ) const;
	bool setOption( EngineOptions::List param, float value );

public:
	int   maxLogLevel;
	int   maxAnisotropy;
	int   shadowMapSize;
	int   sampleCount;
	bool  texCompression;
	bool  sRGBLinearization;
	bool  loadTextures;
	bool  fastAnimation;
	bool  trilinearFiltering;
	bool  wireframeMode;
	bool  debugViewMode;
	bool  dumpFailedShaders;
	bool  gatherTimeStats;
	bool  dumpShaderFile;  // 是否输出shader文件,便于查看
};


// =================================================================================================
// Engine Log
// =================================================================================================

struct LogMessage
{
	std::string  text;
	int          level;
	float        time;

	LogMessage()
	{
	}

	LogMessage( const std::string &text, int level, float time ) :
		text( text ), level( level ), time( time )
	{
	}
};

// =================================================================================================

class EngineLog
{
public:
	EngineLog();

	void writeError( const char *msg, ... );
	void writeWarning( const char *msg, ... );
	void writeInfo( const char *msg, ... );
	void writeDebugInfo( const char *msg, ... );

	bool getMessage( LogMessage &msg );

	uint32 getMaxNumMessages() const { return _maxNumMessages; }
	void setMaxNumMessages( uint32 maxNumMessages ) { _maxNumMessages = maxNumMessages; }
	
protected:
	void pushMessage( const std::string &text, uint32 level );
	void pushMessage( int level, const char *msg, va_list ap );

protected:
	Timer                     _timer;
	char                      _textBuf[2048];
	uint32                    _maxNumMessages;
	std::queue< LogMessage >  _messages;
};


// =================================================================================================
// Engine Stats
// =================================================================================================

struct EngineStats
{
	enum List
	{
		TriCount = 100,
		BatchCount,
		LightPassCount,
		FrameTime,
		AnimationTime,
		GeoUpdateTime,
		ParticleSimTime,
		FwdLightsGPUTime,
		DefLightsGPUTime,
		ShadowsGPUTime,
		ParticleGPUTime,
		TextureVMem,
		GeometryVMem,
		ComputeGPUTime
	};
};

// =================================================================================================

class StatManager
{
public:
	StatManager();
	~StatManager();
	
	bool init();	// Should be called after Renderer initialization

	float getStat( int param, bool reset );
	void incStat( int param, float value );
	Timer *getTimer( int param );
	GPUTimer *getGPUTimer( int param ) const;

protected:
	uint32    _statTriCount;
	uint32    _statBatchCount;
	uint32    _statLightPassCount;

	Timer     _frameTimer;
	Timer     _animTimer;
	Timer     _geoUpdateTimer;
	Timer     _particleSimTimer;
	float     _frameTime;

	GPUTimer  *_fwdLightsGPUTimer;
	GPUTimer  *_defLightsGPUTimer;
	GPUTimer  *_shadowsGPUTimer;
	GPUTimer  *_particleGPUTimer;
	GPUTimer  *_computeGPUTimer;

	friend class ProfSample;
};

// =================================================================================================
// Render Device Capabilities
// =================================================================================================

struct RenderDeviceCapabilities
{
	enum List
	{
		GeometryShaders = 200,
		Tessellation,
		Compute
	};
};

// =================================================================================================

float getRenderDeviceCapabilities( int param );

}
#endif // _egCom_H_
