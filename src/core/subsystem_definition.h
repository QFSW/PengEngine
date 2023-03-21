#pragma once

#define DECLARE_SUBSYSTEM(SubsystemType) \
public: \
	static SubsystemType& get() \
    { \
        return Subsystem::get<SubsystemType>(); \
    } \
