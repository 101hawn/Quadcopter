#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=i686-w64-mingw32-gcc
CCC=i686-w64-mingw32-g++
CXX=i686-w64-mingw32-g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Mingw32-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Windows
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/LoadingRender.o \
	${OBJECTDIR}/MainRender.o \
	${OBJECTDIR}/Renderer.o \
	${OBJECTDIR}/SettingsRender.o \
	${OBJECTDIR}/Transition.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11 -IWindows/include -I/usr/i686-w64-mingw32/sys-root/mingw/include/ -static
CXXFLAGS=-std=c++11 -IWindows/include -I/usr/i686-w64-mingw32/sys-root/mingw/include/ -static

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-LWindows/lib -Wl,-rpath,Windows -lsfml-window -lsfml-audio -lsfml-network -lsfml-main -lsfml-graphics -lsfml-system

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/controller

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/controller: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/controller ${OBJECTFILES} ${LDLIBSOPTIONS} -std=c++11 -lopengl32 -static -static-libgcc -static-libstdc++

${OBJECTDIR}/LoadingRender.o: LoadingRender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LoadingRender.o LoadingRender.cpp

${OBJECTDIR}/MainRender.o: MainRender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MainRender.o MainRender.cpp

${OBJECTDIR}/Renderer.o: Renderer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Renderer.o Renderer.cpp

${OBJECTDIR}/SettingsRender.o: SettingsRender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SettingsRender.o SettingsRender.cpp

${OBJECTDIR}/Transition.o: Transition.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Transition.o Transition.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/controller

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
