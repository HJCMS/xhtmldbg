####################################################################
## RPM Specfile template for xhtmldbg
## Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2009
## Copyright: See COPYING file that comes with this distribution
####################################################################

# norootforbuild

## !!! Willst du dir den Tag versauen - mut du auf OpenSuSE Build Service Pakete bauen !!!

%define devdepend  libtidy-0_99-0-devel libqt4-devel >= 4.7.0 libQtWebKit-devel >= 4.7.0 QTidy-devel >= 0.8.2 libGeoIP-devel >= 1.4.5 mozilla-xulrunner192-devel libqjson-devel >= 0.7.0 kde4-filesystem kdebase4-workspace-devel >= 4.6.0 phonon-devel >= 4.4.4

## @set package name suffix and pkgname
################################
%define lt_version  -1_0-0

Name:           xhtmldbg
Summary:        HTML/XHTML Debugger and Validator
Version:        0.8.13
Release:        253
License:        GPLv3
AutoReqProv:    on
Group:          Productivity/Editors/Other
Url:            http://xhtmldbg.hjcms.de
Source0:        http://gitweb.hjcms.de/cgi-bin/index.cgi/%{name}/snapshot/%{name}-%{version}.tar.bz2
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Supplements:    mimehandler(text/html)
Requires:       qtidyrc >= 0.8.2 oxygen-icon-theme >= 4.4.3 oxygen-icon-theme-scalable >= 4.4.3 bind-utils
BuildRequires:  cmake pkg-config %{devdepend} bind-utils libraptor-devel
BuildRequires:  update-desktop-files brp-check-suse desktop-file-utils qtidyrc >= 0.8.2
BuildRequires:  oxygen-icon-theme >= 4.4.3 oxygen-icon-theme-scalable >= 4.4.3 desktop-data-openSUSE
%if 0%{?suse_version} > 1200
BuildRequires:  libraptor-devel >= 2.0.0
%else
BuildRequires:  libraptor-devel >= 1.4.20
%endif
Vendor:         Heinemann Juergen (Undefined) http://www.hjcms.de
## This package ist not relocatable
Prefix:         /usr

################################
## set enviroment variables
################################
%define _qt_prefix    %{_prefix}
%define _qt_transdir  %{_prefix}/share/qt4/translations

%description
Tidy HTML/XML Validator and Debugger.

Author:
-------------
  Juergen Heinemann (Undefined)

%package -n libxhtmldbg%{lt_version}
Summary:     The xhtmldbg Plugin Interface Library
License:     LGPLv3
Group:       System/Libraries
AutoReqProv: on

%description -n libxhtmldbg%{lt_version}
This Library is used by xhtmldbg for load Plugins

Author:
-------------
  Juergen Heinemann (Undefined)

%package -n libxhtmldbg-devel
Summary:     Development Package for xhtmldbg Plugin Interface Library
License:     LGPLv3
Group:       Development/Languages/C and C++
AutoReqProv: on
Requires:    libxhtmldbg%{lt_version} = %{version}-%{release} %{devdepend}

%description -n libxhtmldbg-devel
This Library is used by xhtmldbg for load Plugins

Author:
-------------
  Juergen Heinemann (Undefined)

%prep

%setup -q -n %{name}-%{version}

%__mkdir_p build

%build

pushd build

cmake -Wno-dev \
  -DCMAKE_CXX_FLAGS:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_C_FLAGS:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_INSTALL_PREFIX:PATH=%{_qt_prefix} \
  -DCMAKE_BUILD_TYPE:STRING=Debug \
  -DCMAKE_SKIP_RPATH:BOOL=ON \
%if 0%{?suse_version} > 1200
  -DFORCE_RAPTOR2:BOOL=ON \
%endif
  -DPHONON_INCLUDE_DIR:PATH=/usr/include/phonon \
  -DPHONON_STL_INCLUDE_DIR:PATH=/usr/include/KDE \
%if %{_lib} == lib64
  -DLIB_SUFFIX:STRING=64 \
%endif
  -DGEOIP_DATABASE_PATH:PATH=/var/lib/GeoIP \
  -DCPACK_PACKAGE_INSTALL_DIRECTORY:PATH="$RPM_BUILD_ROOT" \
  -DOXYGEN_THEME_PATH:PATH=/usr/share/icons/oxygen \
  ../

%__make

popd

%install

pushd build
  %makeinstall
popd

## Update Desktop Files
## % suse_update_desktop_file -u -G %{name}

%find_lang %{name}

%find_lang XQPluginWidgets

%post
##

%postun
##

%files -f %{name}.lang -f XQPluginWidgets.lang
%defattr(-,root,root,-)
%dir /etc/xdg/hjcms.de
%config /etc/xdg/hjcms.de/%{name}.conf
%{_bindir}/%{name}
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/lib*.so
%dir %{_libdir}/%{name}/browser
%dir %{_libdir}/%{name}/designer
%{_libdir}/%{name}/*/lib*.so
%dir %{_datadir}/%{name}
%doc %{_datadir}/%{name}/AUTHORS
%doc %{_datadir}/%{name}/COPYING
%doc %{_datadir}/%{name}/ChangeLog
%doc %{_datadir}/%{name}/NEWS
%doc %{_datadir}/%{name}/README
%{_datadir}/%{name}/*.pot
%dir %{_datadir}/%{name}/schemas
%{_datadir}/%{name}/schemas/*.xsd
%{_datadir}/%{name}/schemas/*.xsl
%{_datadir}/%{name}/schemas/*.dtd
%dir %{_datadir}/%{name}/html
%{_datadir}/%{name}/html/*.html
%{_datadir}/%{name}/html/*.css
%{_datadir}/applications/kde4/%{name}.desktop
%dir %{_datadir}/icons/oxygen/128x128
%dir %{_datadir}/icons/oxygen/128x128/apps
%dir %{_datadir}/icons/oxygen/16x16
%dir %{_datadir}/icons/oxygen/16x16/apps
%dir %{_datadir}/icons/oxygen/192x192
%dir %{_datadir}/icons/oxygen/192x192/apps
%dir %{_datadir}/icons/oxygen/22x22
%dir %{_datadir}/icons/oxygen/22x22/apps
%dir %{_datadir}/icons/oxygen/24x24
%dir %{_datadir}/icons/oxygen/24x24/apps
%dir %{_datadir}/icons/oxygen/32x32
%dir %{_datadir}/icons/oxygen/32x32/apps
%dir %{_datadir}/icons/oxygen/36x36
%dir %{_datadir}/icons/oxygen/36x36/apps
%dir %{_datadir}/icons/oxygen/48x48
%dir %{_datadir}/icons/oxygen/48x48/apps
%dir %{_datadir}/icons/oxygen/64x64
%dir %{_datadir}/icons/oxygen/64x64/apps
%dir %{_datadir}/icons/oxygen/72x72
%dir %{_datadir}/icons/oxygen/72x72/apps
%dir %{_datadir}/icons/oxygen/96x96
%dir %{_datadir}/icons/oxygen/96x96/apps
%dir %{_datadir}/icons/oxygen/scalable
%dir %{_datadir}/icons/oxygen/scalable/apps/
%{_datadir}/icons/oxygen/*/apps/%{name}.png
%{_datadir}/icons/oxygen/*/apps/xhtmldbg.svgz
%{_datadir}/pixmaps/%{name}.xpm
%{_datadir}/dbus-1/*/de.hjcms.xhtmldbg.*

%post -n libxhtmldbg%{lt_version}
/sbin/ldconfig

%postun -n libxhtmldbg%{lt_version}
/sbin/ldconfig

%files -n libxhtmldbg%{lt_version}
%defattr(-,root,root,-)
%{_libdir}/libxhtmldbg-1.0.so.*

%post -n libxhtmldbg-devel
##

%postun -n libxhtmldbg-devel
##

%files -n libxhtmldbg-devel
%defattr(-,root,root,-)
%{_libdir}/libxhtmldbg-1.0.so
%{_libdir}/pkgconfig/xhtmldbg.pc
%dir %{_includedir}/xhtmldbg-1.0
%{_includedir}/xhtmldbg-1.0/*.h
%dir %{_includedir}/xhtmldbg-1.0/designer
%{_includedir}/xhtmldbg-1.0/designer/*.h
%dir %{_datadir}/%{name}/plugintemplate
%dir %{_datadir}/%{name}/designertemplate
%{_datadir}/%{name}/*/*.h
%{_datadir}/%{name}/*/*.txt
%{_datadir}/%{name}/*/*.cpp

%clean
test -d "$RPM_BUILD_ROOT" && rm -rf $RPM_BUILD_ROOT

%changelog
* Mon Apr 18 2011 - Heinemann Juergen (Undefined) http://www.hjcms.de
- Update to 0.8.13

* Mon Mar 14 2011 - Heinemann Juergen (Undefined) http://www.hjcms.de
- Update to 0.8.12.rc17

* Thu Feb 03 2011 - Heinemann Juergen (Undefined) http://www.hjcms.de
- Update to 0.8.12.rc1

* Sun Sep 05 2010 - Heinemann Juergen (Undefined) http://www.hjcms.de
- Rebuild to .rc10
- Bugfix Invalid CMake Flags

* Sun Jul  4 2010 - Heinemann Juergen (Undefined) http://www.hjcms.de
- Rebuild Bugfix Icon Theme Location

* Sun Jun 27 2010 - Heinemann Juergen (Undefined) http://www.hjcms.de
- Rebuild 0.8.8.rc3 for new OpenSuSE Build Service

