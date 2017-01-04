import sys,os,commands
import  os
import re
import sys

def SWIGSharedLibrary(env, library, sources, **args):
  swigre = re.compile('(.*).i')
  if env.WhereIs('swig') is None:
    sourcesbis = []
    for source in sources:
      cName = swigre.sub(r'\1_wrap.c', source)
      cppName = swigre.sub(r'\1_wrap.cc', source)
      if os.path.exists(cName):
        sourcesbis.append(cName)
      elif os.path.exists(cppName):
        sourcesbis.append(cppName)
      else:
        sourcesbis.append(source)
  else:
    sourcesbis = sources
 
  if 'SWIGFLAGS' in args:
    args['SWIGFLAGS'] += ['-python']
  else:
    args['SWIGFLAGS'] = ['-python'] + env['SWIGFLAGS']
  args['SHLIBPREFIX']=""
  if sys.version >= '2.5':
    args['SHLIBSUFFIX']=".pyd"
 
  cat=env.SharedLibrary(library, sourcesbis, **args)
  return cat
 


# environment
#import xdaq
#print xdaq.INCLUDES
#print xdaq.LIBRARY_PATHS
#print xdaq.LIBRARIES

#print "----------------------------------------------"
Decider('MD5-timestamp')
XDAQ_ROOT="/opt/xdaq"
NETLINK=os.environ["HOME"]+"/SDHCAL/netLink/"
if not os.path.exists(NETLINK):
   NETLINK="/home/acqilc//SDHCAL/netLink/"
DHCAL_ROOT=os.path.abspath("..")
try:
  DIM_ROOT=os.environ["DIMDIR"]
  Use_Dim=True
except:
  Use_Dim=False

fres=os.popen('uname -r')
kl=fres.readline().split(".")

platform="UBUNTU"
if (kl[len(kl)-1][0:3] == 'el5'):
    platform="SLC5"

if (kl[len(kl)-2][0:3] == 'el6'):
    platform="SLC6"
print kl[len(kl)-2][0:3]
fres=os.popen('uname -p')
kp=fres.readline()
osv=kp[0:len(kp)-1]

print platform,osv

Bit64=False
Bit64=os.uname()[4]=='x86_64'

Arm=os.uname()[4]=='armv7l'

if Arm or platform=="UBUNTU":
  boostsystem='boost_system'
  boostthread='boost_thread'
else:
  boostsystem='boost_system-mt'
  boostthread='boost_thread-mt'
  
# includes
INCLUDES=['include',"/usr/include/boost141/","/usr/include/jsoncpp","/usr/local/include","/usr/local/include/mongoose",NETLINK+"include"]


INCLUDES.append(commands.getoutput("python -c 'import distutils.sysconfig as conf; print conf.get_python_inc()'"))

CPPFLAGS=["-O6","-DLINUX", "-DREENTRANT" ,"-Dlinux", "-DLITTLE_ENDIAN__ ", "-Dx86",  "-DXERCES=2", "-DDAQ_VERSION_2"]

#Library ROOT + some of XDAQ + DB 



LIBRARIES=['pthread',  'm','dl', 'stdc++','jsoncpp','z','mongoose','curl','netlink',boostsystem,boostthread]



#Library path XDAQ,DHCAL and ROOT + Python
if (Bit64):
	LIBRARY_PATHS=["/usr/lib64","/usr/local/lib","/usr/lib64/boost141"]
else:
  LIBRARY_PATHS=["/usr/lib","/usr/local/lib","/usr/lib/boost141"]
LIBRARY_PATHS.append(commands.getoutput("python -c 'import distutils.sysconfig as conf; print conf.PREFIX'")+"/lib")

LIBRARY_PATHS.append(NETLINK+"lib")

#link flags
#LDFLAGS=["-fPIC","-dynamiclib"]
LDFLAGS=["-fPIC","-dynamiclib"]


# SWIG
SWIGSF=["-c++","-classic"]

for i in INCLUDES:
    SWIGSF.append("-I"+i)
print SWIGSF

# Create the Environment
env = Environment(CPPPATH=INCLUDES,CPPFLAGS=CPPFLAGS,LINKFLAGS=LDFLAGS, LIBS=LIBRARIES,LIBPATH=LIBRARY_PATHS,SWIGFLAGS=SWIGSF)

#print "CC is:",env.subst('$CPPPATH')

env['BUILDERS']['PythonModule'] = SWIGSharedLibrary


# Library source
LIBRARY_SOURCES=Glob("#src/*.cxx")

#Shared library
lptt=env.SharedLibrary("#lib/ptt",LIBRARY_SOURCES)

#Python module
#MYLIBS=["levbdim",'pthread',  'm', 'stdc++',boostsystem,boostthread]
#_levbdim=env.PythonModule('_Llevbdim', ['Llevbdim.i'],LIBPATH="#lib",LIBS=MYLIBS)

#env.Install(DHCAL_ROOT+"/opt/dhcal/lib",levbdim)
###env.Install("/opt/dhcal/lib",levbdim)
###env.Install("/opt/dhcal/include/readout",myinc)

###env.Alias('install', [DHCAL_ROOT+"/opt/dhcal/lib","/opt/dhcal/lib","/opt/dhcal/include/rpiccc"])
#env.Alias('install', [DHCAL_ROOT+"/opt/dhcal/lib"])

EXE_LIBPATH=LIBRARY_PATHS
EXE_LIBPATH.append("#lib")
EXE_LIBS=LIBRARIES
EXE_LIBS.append("ptt")


#plugbase=env.SharedLibrary("lib/basicwriter",source="src/basicwriter.cc",LIBPATH=EXE_LIBPATH,LIBS=EXE_LIBS)
#EXE_LIBS.append("basicwriter")
dums=env.Program("bin/dummyS",source="test/dummyServer.cxx",LIBPATH=EXE_LIBPATH,LIBS=EXE_LIBS)
china=env.Program("bin/chinaD",source="test/TdcDaemon.cxx",LIBPATH=EXE_LIBPATH,LIBS=EXE_LIBS)
po=env.Program("bin/pod",source="test/PostDaemon.cxx",LIBPATH=EXE_LIBPATH,LIBS=EXE_LIBS)
tb=env.Program("bin/tbd",source="test/testBuilder.cxx",LIBPATH=EXE_LIBPATH,LIBS=EXE_LIBS)
tf=env.Program("bin/tfd",source="test/testFeeder.cxx",LIBPATH=EXE_LIBPATH,LIBS=EXE_LIBS)
job=env.Program("bin/jobd",source="test/JobDaemon.cxx",LIBPATH=EXE_LIBPATH,LIBS=EXE_LIBS)

#dumb=env.Program("bin/dummyB",source="test/dummyBuilder.cxx",LIBPATH=EXE_LIBPATH,LIBS=EXE_LIBS)
#ljcb=env.Program("bin/ljc",source="test/ljc.cxx",LIBPATH=EXE_LIBPATH,LIBS=EXE_LIBS)	

