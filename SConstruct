import glob
#ode_path = '/home/hmengist/ode-0.13/'
ode_path = '/home/hmengist/include'
tbb_path = '/apps/l_ics_2012.0.032/composer_xe_2011_sp1.6.233/tbb/include'
boost_path='/home/hmengist/sferes/include'
mpi_path='/apps/OPENMPI/openmpi-1.6.2_intel/include'
#mpi_path ='/apps/OPENMPI/intel/14.0.0/1.8.1/include'
path = ['/usr/local/bin', '/bin', '/usr/bin','/apps/INTEL/2013/composer_xe_2013_sp1.0.080/bin/intel64']
INTEL_LICENSE_FILE=['/apps/intel_flexlm/server.lic']
env = Environment(CCFLAGS=' -DdTRIMESH_ENABLED -DdDOUBLE -DGRAPHICS -O3 -g -I./include -I%s -lpthread -I%s -I%s -I%s' % (ode_path,tbb_path,boost_path, mpi_path),ENV = {'PATH' : path, 'INTEL_LICENSE_FILE' : INTEL_LICENSE_FILE})
env.Replace(CC = "icc")
env.Replace(CXX="icpc")


#env.AppendENVPath('CPLUS_INCLUDE_PATH', ode_path+'include')

#current=['biped.cpp',"biped_stuff.cpp","ConfigFile.cpp"] 
#rtneat=glob.glob('rtneat/*.cpp')

current=glob.glob('*.cpp')

allsrc=current #+rtneat
allsrc.remove("mazeApp.cpp")
allsrc.remove("mazeDlg.cpp")
#allsrc.remove("elites_population.cpp")
#allsrc.remove("noveltyexp_mapelite.cpp")
#allsrc.remove("maze.cpp")
#allsrc.remove("noveltyexp.cpp") 
#allsrc.remove("biped_mapelite.cpp")
allsrc.remove("biped_stuff.cpp") # untill the parallism works for noveltyexp.cpp, it is removed




#print allsrc
exe = 'biped_fdo'
env.Program(exe, allsrc,LIBS=['m','ode','pthread','tbb','boost_mpi','mpi','imf','intlc','irng','intlc','boost_serialization','svml','mpi_cxx'],LIBPATH=['.','/home/hmengist/lib','/usr/lib/','/usr/local/lib','/apps/l_ics_2012.0.032/composer_xe_2011_sp1.6.233/tbb/lib/intel64/cc4.1.0_libc2.4_kernel2.6.16.21','/home/hmengist/sferes/lib','/apps/OPENMPI/intel/14.0.0/1.8.1/lib'])

print env["CXX"]
print env['ENV']['PATH']
print exe, "IS BUILT"
