#!/bin/sh
#
# bootstrap autotools
#
# $Id: autogen.sh,v 1.1.1.1 2006/10/27 10:35:31 smoln Exp $

ag_unused_vars='DEFS CPPFLAGS LDFLAGS LIBS'
ag_unused_dirs='sbindir libexecdir datadir sysconfdir sharedstatedir 
                localstatedir oldincludedir infodir mandir'

# Get variables passed to autogen.sh through environment
ag_initialize()
{
  # Configure template
  ag_configure_in='configure.in'

  # Directories
  ag_prefix=${prefix-$ag_default_prefix}
  
  # Operating-system/architecture
  ag_host=${host}
  ag_build=${build}
  ag_target=${target}

  # Standard shell programs
  ag_rm=${RM-rm -f}
  ag_sed=${SED-sed}

  # Helper programs
  ag_aclocal=${ACLOCAL-aclocal}
  ag_autoconf=${AUTOCONF-autoconf}
  ag_autoheader=${AUTOHEADER-autoheader}
  ag_libtoolize=${LIBTOOLIZE-libtoolize}
  ag_automake=${AUTOMAKE-automake}
  
  # Preprocessors/compilers/linker
  ag_cc=${CC}
  ag_cpp=${CPP}
  ag_cxx=${CXX}
  ag_cxxcpp=${CXXCPP}

  # Preprocessor/compiler/linker flags
  ag_cflags=${CFLAGS}
  ag_ldflags=${LDFLAGS}
  ag_cppflags=${CPPFLAGS}
  ag_cxxflags=${CXXFLAGS}
}

# Get configuration settings from configure.in
ag_parse()
{
  # Get the package name
  ag_package_name=$(ag_run_shell $ag_sed -n \
    '/AC_INIT/ { s/.*(\[\?\([-_/a-z0-9]\+\)\]\?[,)].*/\1/i; p }' \
    $ag_configure_in)

  # Get the package version
  ag_package_version=$(ag_run_shell $ag_sed -n \
    '/AC_INIT/ { s/.*(\[\?[-_/a-z0-9]\+\]\?[,)][ \t]*\[\?\([-_.a-z0-9]\+\)\]\?.*/\1/i; p }' \
    $ag_configure_in)

  # Get the default prefix
  ag_default_prefix=$(ag_run_shell $ag_sed -n \
    '/AC_PREFIX_DEFAULT/ { s/.*(\[\?\([-_/a-z0-9]\+\)\]\?).*/\1/i; p }' \
    $ag_configure_in)
    
  # Get the config aux dir
  ag_config_aux_dir=$(ag_run_shell $ag_sed -n \
    '/AC_CONFIG_AUX_DIR/ { s/.*(\[\?\([-_/a-z0-9]\+\)\]\?).*/\1/i; p }' \
    $ag_configure_in)
    
  ag_aux_dir=${ag_config_aux_dir:+$ag_config_aux_dir/}

  # Get the config files
  ag_config_files=$(ag_run_shell $ag_sed -n \
    '/AC_CONFIG_FILES/ { 
      s/.*AC_CONFIG_FILES(\?[\n\t ]*\[\?//
      
      :l
      /)/! { N; b l } 

      s/\]\?[\n\t ]*).*//
      s/[\n\t ]\+/ /g
      
      p
    }' \
    $ag_configure_in)

  # Get the config headers
  ag_config_headers=$(ag_run_shell $ag_sed -n \
    '/AC_CONFIG_HEADER/ { 
      s/.*AC_CONFIG_HEADERS\?(\?[\n\t ]*\[\?//
      
      :l
      /)/! { N; b l } 

      s/\]\?[\n\t ]*).*//
      s/[\n\t ]\+/ /g

      p
    }' \
    $ag_configure_in)

  # Disable shared libs by default?
  ag_disable_shared=$(ag_run_cond grep -q '^AM_DISABLE_SHARED' $ag_configure_in)

  # Disable static libs by default?
  ag_disable_static=$(ag_run_cond grep -q '^AM_DISABLE_STATIC' $ag_configure_in)
  
  # Use libtool?
  ag_use_libtool=$(ag_run_cond grep -q '^A[CM]_PROG_LIBTOOL' $ag_configure_in)
  
  # Use C++ compiler?
  ag_use_cxx=$(ag_run_cond grep -q '^AC_PROG_CXX' $ag_configure_in)

  # Use Fortran77 compiler?
  ag_use_f77=$(ag_run_cond grep -q '^AC_PROG_F77' $ag_configure_in)
}

# Parse package version string
ag_version()
{
  local IFS='.'

  set -- $ag_package_version
  
  ag_version_major=$(echo $1 | sed 's,.*\([0-9]\+\).*,\1,')
  ag_version_minor=$(echo $2 | sed 's,.*\([0-9]\+\).*,\1,')
  ag_version_patch=$(echo $3 | sed 's,.*\([0-9]\+\).*,\1,')
  
  ag_library_version=$((${ag_version_major:-1} + ${ag_version_minor:-0})):${ag_version_patch:-0}:${ag_version_minor:-0}
}

# Prepare file lists
ag_files()
{
  # Make a list of all Makefile.am
  unset ag_makefiles
  unset ag_makefile_ams
  unset ag_makefile_ins

  for ag_config_file in $ag_config_files
  do
    if [ -f "$ag_config_file.am" ]; then
      ag_makefiles="${ag_makefiles+$ag_makefiles }$ag_config_file"
      ag_makefile_ams="${ag_makefile_ams+$ag_makefile_ams }$ag_config_file.am"
      ag_makefile_ins="${ag_makefile_ins+$ag_makefile_ins }$ag_config_file.in"
    fi
  done
  
  # Scan the Makefile.am's for all SOURCES and HEADERS
  unset ag_sources
  unset ag_headers
  unset ag_programs
  unset ag_libraries
  
  for ag_makefile_am in $ag_makefile_ams
  do
    local mak=${ag_makefile_am##*/}
    local dir=${ag_makefile_am%$mak}
    
    # Get list of source files
    local src=$($ag_run_shell $ag_sed -n \
      '/^[ \t]*[_0-9a-z]*_\?SOURCES[ \t]*=/ {
        s,.*=[ \t]*,,
        :l; /\\$/ { N; b l }
        s,[\t\n \\]\+, ,g
        p
      }' $ag_makefile_am)
      
    for file in $src
    do
      if [ -f "$dir$file" ]; then
        ag_sources="${ag_sources+$ag_sources }$dir$file"
      fi
    done
    
    # Get list of header files
    local hdr=$($ag_run_shell $ag_sed -n \
      '/^[ \t]*[_0-9a-z]*_\?HEADERS[ \t]*=/ {
        s,.*=[ \t]*,,
        :l; /\\$/ { N; b l }
        s,[\t\n \\]\+, ,g
        p
      }' $ag_makefile_am)
      
    for file in $hdr
    do
      if [ -f "$dir$file" ]; then
        ag_headers="${ag_headers+$ag_headers }$dir$file"
      fi
    done
    
    # Get list of programs
    local bin=$($ag_run_shell $ag_sed -n \
      '/^[ \t]*[_0-9a-z]*_\?PROGRAMS[ \t]*=/ {
        s,.*=[ \t]*,,
        :l; /\\$/ { N; b l }
        s,[\t\n \\]\+, ,g
        p
      }' $ag_makefile_am)
      
    for file in $bin
    do
      ag_programs="${ag_programs+$ag_programs }$dir$file"
    done
    
    # Get list of libraries
    local lib=$($ag_run_shell $ag_sed -n \
      '/^[ \t]*[_0-9a-z]*_\?L\?T\?LIBRARIES[ \t]*=/ {
        s,.*=[ \t]*,,
        :l; /\\$/ { N; b l }
        s,[\t\n \\]\+, ,g
        p
      }' $ag_makefile_am)
      
    for file in $lib
    do
      ag_libraries="${ag_libraries+$ag_libraries }$dir$file"
    done
  done
}

# Run a tool
ag_run_tool()
{
  echo "+ $@" 1>&2
  "$@" 2>&1
}

# Run a shell script tool
ag_run_shell()
{
#  echo "+ $@" 1>&2
  "$@" 2>&1
}

# Run a shell script tool and output the inverse exit code.
ag_run_cond()
{
#  echo "+ $@" 1>&2  
  "$@" 1>&2 &&
  echo true ||
  echo false
}

# Run a tool
ag_run_quiet()
{
  echo "+ $@" 1>&2  
  "$@" 2>/dev/null
}

# Run aclocal to create the aclocal.m4 macro collection.
ag_cmd_aclocal()
{
  # Output file.
  local ag_output='aclocal.m4'
  
  # Remove output file.
  ag_run_shell $ag_rm $ag_output
  
  # Run aclocal
  ag_run_quiet $ag_aclocal "$@" || return $?
  
  # libtool sometimes adds unnecessary macro crap
  if $ag_use_libtool; then
  
    # No C++ support?
    if ! $ag_use_cxx; then
      ag_run_shell $ag_sed -i \
        '/^AC_DEFUN.*AC_LANG_CXX/ {
          :l; /\n\])#/! { N; b l }
          s/,.*/, [:])/
        }' \
        $ag_output
    fi

    # No fortran 77 support?
    if ! $ag_use_f77; then
      ag_run_shell $ag_sed -i \
        '/^AC_DEFUN.*AC_LANG_F77/ {
          :l; /\n\])#/! { N; b l }
          s/,.*/, [:])/
        }' \
        $ag_output
    fi

  fi
}

# Run autoconf to create the configure script.
ag_cmd_autoconf()
{
  # Output file.
  local ag_output='configure'
  
  # Remove output file.
  ag_run_shell $ag_rm $ag_output

  # Run autoconf
  ag_run_tool $ag_autoconf "$@" || return $?

  # Don't really generate this header
  ag_run_shell $ag_sed -i \
    '/ac_config_headers=/ s, *src/jscpucfg.h,,' \
    $ag_output

  # Turn of dependency tracking per default
  ag_run_shell $ag_sed -i \
    '/tracking was given/ a\: ${enable_dependency_tracking:=no}\ ' \
    $ag_output

  # Check if any source file tests HAVE_CONFIG_H
  if ! grep -q 'HAVE_CONFIG_H' $ag_sources $ag_headers; then
    ag_run_shell $ag_sed -i \
      '/HAVE_CONFIG_H/ s,^,#,' \
      $ag_output
  fi
  
  # Some fixes for libtool
  if $ag_use_libtool; then
    ag_run_shell $ag_sed -i \
      "/lt_prog_compiler_static works\"/ i\: \${lt_prog_compiler_static:=-static}
       /soname_spec='.*SED.*'/ s,=.*,='\\\${name}\${release}\\\$shared_ext'," \
      $ag_output   
  fi
  
  # Remove unused variables and directories
  subst=''
  remove=''
  
  for var in $ag_unused_vars
  do
    subst="$subst
      /^  $var / { :l; N; /\n   / { s,\n   .*,,;  b l }; D }
      /^ac[_a-z]*env[_a-z]*$var[_a-z]*=/ d
 #     /\$$var/ { s, \$$var,,g; s,\$$var ,, }"
      
    remove="$remove
      s,\([' ]\)$var\([' ]\),\1\2,"
  done
  
  for dir in $ag_unused_dirs
  do
    subst="$subst
      /^  --$dir=DIR/ d
      /^$dir=/ d
      /^  \-$dir/ { :l; N; /;;$/! b l; d }
      / $dir/ s, $dir,,
      /^s,@$dir@/ d"
    remove="$remove
      s,\([' ]\)$dir\([' ]\),\1\2,"
  done
  
  ag_run_shell $ag_sed -i \
    "$subst
    /^ac_subst_vars=/ {
      $remove
      s,[ \t]\+, ,g
    }" $ag_output
  
  return 0

  sed -i -e "" \
         -e 's,\(ldconfig.*$libdir\),\1 2>/dev/null,' \
         -e  \
         -e 's,test \-z "$F77",false,' \
         -e 's,test \-n "$ac_tool_prefix",false,' \
         -e '/F77 accepts \-g/ s,^,#,' \
         -e '/echo.*ac_cv_prog_f77_g/ s,echo,#echo,' \
         -e 's,test "${ac_cv_prog_f77_g+set}" = set,false,' \
         -e 's,test $ac_cv_prog_f77_g = yes,false,' \
         -e '/^tagnames.*CXX$/ s,^,#,' \
         -e '/^tagnames.*F77$/ s,^,#,' \
         -e '/ac_cv_path_install=/ s,$as_dir/,,' \
         -e '/) *ac_INSTALL=$INSTALL/ s,),| *install | *"install "* ),' \
         -e '/test "$GCC" = yes/ { N; /-fno-builtin/ s,if.*then,if false; then, }' \
         -e '/^ac_ext=cc$/ { :lp; N; /G77=/! b lp; d; }' \
         -e '/^ac_subst_vars=/ {
            s,[ \t]*[A-Za-z0-9_]*F77[A-Za-z0-9_]*[ \t]*, ,g
            s,[ \t]*[A-Za-z0-9_]*CXX[A-Za-z0-9_]*[ \t]*, ,g
            s,[ \t]\+, ,g
            }' \
         configure

  sed -i -e '/^if test \-z.*am__fastdepCXX.*then$/ { :lp; N; /fi$/! b lp; d; }' \
         configure

#        -e '/^if test.*am__fastdepCXX_TRUE"/ { :lp; N; /fi$/! b lp; d; }' \
#          -e '/^[ \t]am__fastdepCXX/ d' \
#         -e '/^s,@am__fastdepCXX/ d' \
#         -e '/^ac_ext=cc$/ { :lp; N; /lt_prog_compiler_static_CXX=$/! b lp; d; }' \
#         -e '/^ac_ext=F$/ { :lp; N; /G77=/! b lp; d; }' \
#
}

# Run autoheader to create the configuration header templates.
ag_cmd_autoheader()
{
  # Configuration header templates
  local ag_config_h_in='config/config.h.in'
  
  # Output files.
  local ag_output="$ag_config_h_in $ag_src_jsconfig_h_in"

  # Remove output files.
  ag_run_shell $ag_rm $ag_output
  
  # Run autoheader.
  ag_run_tool $ag_autoheader "$@"  
}

# Run libtoolize to create the ltmain.sh libtool script.
ag_cmd_libtoolize()
{
  # libtool generation script
  local ag_ltmain_sh="${ag_aux_dir}ltmain.sh"
  local ag_config_guess="${ag_aux_dir}config.guess"
  local ag_config_sub="${ag_aux_dir}config.sub"

  # Output files.
  local ag_output="$ag_ltmain_sh $ag_config_guess $ag_config_sub"

  # Create config aux dir
  if [ "$ag_config_aux_dir" ]; then
    ag_run_shell mkdir -p "$ag_config_aux_dir"
  fi

  # Remove output files.
  ag_run_shell $ag_rm $ag_output
  
  # Run libtoolize.
  ag_run_tool $ag_libtoolize "$@"
  
  # Turn the --silent argument into --verbose, inverse functionality
  ag_run_shell $ag_sed -i \
    '/^show=/ s,=.*$,=:,
     /-\-quiet.*-\-silent/ { 
       s,quiet.*silent,verbose,
       n
       s,show=.*,show="$echo", 
     }
     /-\-quiet / d
     /-\-silent / s,silent ,verbose,
     /don.t print/ s,don.t ,,' \
    $ag_ltmain_sh
}

# Run automake to generate Makefile templates.
ag_cmd_automake()
{
  # Parse options
  unset ag_options
  
  while [ $# -gt 0 ]; do
    case $1 in
      -* | --*)
        ag_options="${ag_options+$ag_options }$1"
        shift
        ;;
      *)
        break
        ;;
    esac
  done

  # Makefiles
  if [ $# -gt 0 ]; then
    ag_makefiles="$*"
    
    unset ag_makefile_ins
    
    for ag_makefile in $ag_makefiles; do
      ag_makefile_ins="${ag_makefile_ins+$ag_makefile_ins }$ag_makefile.in"
    done
  fi
  
  ag_output="$ag_makefile_ins"

  # Remove output files.
  ag_run_shell $ag_rm $ag_output
  
  # Run automake.
  ag_run_tool $ag_automake $ag_options "$@" || return $?
  
  # Replace bootstrap tools
  ag_run_shell $ag_sed -i \
    '/^ACLOCAL =/ s,=.*,= ./autogen.sh aclocal,
     /^AUTOCONF =/ s,=.*,= ./autogen.sh autoconf,
     /^AUTOMAKE =/ s,=.*,= ./autogen.sh automake,
     /^AUTOHEADER =/ s,=.*,= ./autogen.sh autoheader,' \
     $ag_makefile_ins

  # Change the command lines
  ag_run_shell $ag_sed -i \
    ':l; /\\$/ { N; b l }
     /^[_A-Z]\+ =/ {
       s, *\-\-tag=[A-Z]\+,,
       s, *$(DEFS),,g
       s, *$(DEFAULT_INCLUDES),,g
       s, *$(CPPFLAGS),,g
      
       /^[A-Z]*COMPILE/ s,[ \\\n\t]\+, ,g
    }' \
    $ag_makefile_ins

  # Clear references to unspecified variables
  local substs=""
  
  for target in $ag_libraries $ag_programs
  do
    local name=${target##*/}
    
    for var in CPPFLAGS CFLAGS LDFLAGS
    do
      # if program/lib_VARIABLE does not exist, then we remove all references to it
      substs="$substs"'
      /\n@\?[_A-Z]*@\?'${name//./_}_${var}' =/! { s, *$('${name//./_}_${var}'),,g }'
    done
    
    substs="$substs"'
    /am_'${name//./_}'_rpath =[ \t]*\n/ { s, *$(am_'${name//./_}'_rpath),,g }'
  done
  
  ag_run_shell $ag_sed -i \
    ':l; $! { N; b l }

     /\nAM_CPPFLAGS =/! { s, *$(AM_CPPFLAGS),,g }
     /\nAM_CFLAGS =/! { s, *$(AM_CFLAGS),,g }
     /\nAM_CXXFLAGS =/! { s, *$(AM_CXXFLAGS),,g }
     /\nAM_LDFLAGS =/! { s, *$(AM_LDFLAGS),,g }
     /\nAM_MAKEFLAGS =/! { s, *$(AM_MAKEFLAGS),,g }
     /\nAM_CTAGSFLAGS =/! { s, *$(AM_CTAGSFLAGS),,g }
     /\nAM_ETAGSFLAGS =/! { s, *$(AM_ETAGSFLAGS),,g }

    '"$substs" \
    $ag_makefile_ins

  # Merge specialized variables with generic
  local substs=""
  
  for target in $ag_libraries $ag_programs
  do
    local name=${target##*/}
    
    substs="$substs"'
      /^'${name//./_}'_CFLAGS =/ { s, *$(CFLAGS),,; s,=,= $(CFLAGS), }
      /$('${name//./_}'_CFLAGS)/ { s, *$(CFLAGS),, }
      /^'${name//./_}'_LDFLAGS =/ { s, *$(LDFLAGS),,; s,=,= $(LDFLAGS), }
      /$('${name//./_}'_LDFLAGS)/ { s, *$(LDFLAGS),, }
      /^'${name//./_}'_LDADD =/ { s, *$(LIBS),,; s,$, $(LIBS), }
      /$('${name//./_}'_LDADD)/ { s, *$(LIBS),, }'
  done
  
  ag_run_shell $ag_sed -i \
    ':l; /\\$/ { N; b l }'"$substs" \
    $ag_makefile_ins
    
  # Fix up some linker stuff
  ag_run_shell $ag_sed -i \
    ':l; /\\$/ { N; b l }
     
    /^LINK =/ {
      s, *-o $@,,
      s, *$(CFLAGS),,
      s, *$(LDFLAGS),,
      s,[\t\n \\]\+, ,g
      s,[\t\n \\]*$,, 
    }

    /$(LINK)/ {
      s,\($([0-9A-Z]*LINK)\),\1 \-o $@,
    }

    /^CCLD =/ { 
      s, *$(LDFLAGS),,
      s,$, $(LDFLAGS), 
    }' \
    $ag_makefile_ins
    
  # Fix up program/library specific variables
#  for target in $ag_libraries $ag_programs
#  do
#    local name=${target##*/}
#
#    ag_run_shell $ag_sed -i \
#      '/^'${name//./_}'_[_A-Z]\+ =/ {
#        s,^'${name//./_}'_\([_A-Z]\+\) =,& = $(\1),
#      }' \
#      $ag_makefile_ins
#  done

  # Don't specify -o on compilation
  ag_run_shell $ag_sed -i \
    '/\t$(COMPILE)/ s,-o $@ *,,
     /\t$(LTCOMPILE)/ s,-o $@ *,,
     /\t$(CXXCOMPILE)/ s,-o $@ *,,
     /\t$(LTCXXCOMPILE)/ s,-o $@ *,,' \
    $ag_makefile_ins

  # Fix specialized compilation targets
  ag_run_shell $ag_sed -i \
    '/^@\?[_0-9A-Za-z]*@\?\ti\?f\? \?$(CC)/ {
       s, *$(DEFS),,
       s, *$(DEFAULT_INCLUDES),,
       s, *$(CPPFLAGS),,
     }

     /^@\?[_0-9A-Za-z]*@\?\ti\?f\? \?$(CXX)/ {
       s, *$(DEFS),,
       s, *$(DEFAULT_INCLUDES),,
       s, *$(CPPFLAGS),,
     }' \
    $ag_makefile_ins
    
  # Remove unused variables
  subst=''
    
  for var in $ag_unused_vars; do
    subst="$subst
    /^$var = @$var@$/ d
    / \$($var)/ s, \$($var),,g"
  done
    
  ag_run_shell $ag_sed -i \
    "$subst" \
    $ag_makefile_ins

  # Remaining tweaks
  ag_run_shell $ag_sed -i \
    "/echo ['\"]/ s,\(echo ['\"]\) ,\1,
     /\$(SHELL)/ s,\$(SHELL) *,,
     /\$(AUTOMAKE)/ s,\(\-\-foreign\) *,\1 ,
     /\-version\-info/ s,\(\-version\-info\)[ \t]\+[0-9]\+:[0-9]\+:[0-9]\+,\1 $ag_library_version," \
    $ag_makefile_ins
}
 
# launch the configure script
ag_cmd_configure()
{
  ag_run_tool ./configure \
    ${ag_cv_prefix:+--prefix=${ag_cv_prefix}} \
    ${ag_cv_host:+--host=${ag_cv_host}} \
    ${ag_cv_build:+--build=${ag_cv_build}} \
    ${ag_cv_maintainer_mode:+--$ag_cv_maintainer_mode-maintainer-mode} \
    ${ag_cv_dependency_tracking:+--$ag_cv_dependency_tracking-dependency-tracking} \
    ${ag_cv_debug:+--$ag_cv_debug-debug} \
    ${ag_cv_werror:+--$ag_cv_werror-werror} \
    ${ag_cv_verbose:+--$ag_cv_verbose-verbose} \
    "$@"
}

# set some important vars
ag_setvars()
{
  ag_system=$(uname -s)
  
  case $ag_system in
    'CYGWIN_NT-5.1' | 'WindowsNT')
      : ${prefix=c:/dev-cpp}
      : ${host=i686-pc-mingw32}
      : ${build=i686-pc-mingw32}
      : ${CC=$prefix/bin/gcc}
      : ${CXX=$prefix/bin/g++}
      ;;
    *)
      : ${prefix=/usr}
      : ${host=i686-pc-linux-gnu}
      : ${build=i686-pc-linux-gnu}
      : ${CC=gcc}
      : ${CXX=g++}
      ;;
  esac

  : ${CFLAGS="-g -ggdb -O2"}
  : ${CXXFLAGS="-g -ggdb -O2"}
  : ${CPPFLAGS=""}
  
  export CC CXX CFLAGS CXXFLAGS CPPFLAGS
}  

# main script
ag_main()
{
  ag_cmd_aclocal -I m4 &&
  ag_cmd_autoconf &&
  ag_cmd_autoheader &&
  ag_cmd_libtoolize --force --copy &&
  ag_cmd_automake --add-missing --foreign &&
  ag_cmd_configure "$@"
}

 
# extract action from arguments
ag_action=$1

case $ag_action in
  */* | --* | *=* | "" )
    ag_action=""
    ;;
  *)
    shift
    ;;
esac

# Initialize autogen variables
ag_initialize

# Parse the configure template to get static settings
ag_parse

# Prepare file lists
ag_files

# Prepare version info strings
ag_version

# Default arguments
ag_cv_prefix="$prefix"
ag_cv_host="$host"
ag_cv_build="$build"

ag_cv_maintainer_mode='enable'
ag_cv_dependency_tracking='enable'
ag_cv_debug='enable'
ag_cv_werror='enable'
ag_cv_verbose='enable'

# Parse arguments
ag_resetarg=0

for ag_arg
do
  if [ "$ag_resetarg" = 0 ]; then
    set --
    ag_resetarg=1
  fi

  case $ag_arg in
    --prefix=*) ag_cv_prefix=${ag_arg#*=} ;;
    --host=*) ag_cv_host=${ag_arg#*=} ;;
    --build=*) ag_cv_build=${ag_arg#*=} ;;
    
    --enable-maintainer-mode*) ag_cv_maintainer_mode='enable' ;;
    --disable-maintainer-mode*) ag_cv_maintainer_mode='' ;;

    --enable-dependency-tracking*) ag_cv_dependency_tracking='enable' ;;
    --disable-dependency-tracking*) ag_cv_dependency_tracking='' ;;
  
    --enable-debug*) ag_cv_debug='enable' ;;
    --disable-debug*) ag_cv_debug='' ;;

    --enable-werror*) ag_cv_werror='enable' ;;
    --disable-werror*) ag_cv_werror='' ;;

    --enable-verbose*) ag_cv_verbose='enable' ;;
    --disable-verbose*) ag_cv_verbose='' ;;

    --tight*)
      ag_cv_maintainer_mode=''
      ag_cv_dependency_tracking=''
      ag_cv_debug=''
      ag_cv_werror=''
      ag_cv_verbose=''
      ;;

    *) set -- "$@" "$ag_arg" ;;
  esac
done                                                  

# Set environment variables
ag_setvars

# Set trap handler
trap 'rm -f $ag_output; exit 2' HUP INT TERM QUIT

if ! test "$ag_action"; then
  ag_main "$@"
  exit $?
fi

ag_cmd_${ag_action} "$@"
