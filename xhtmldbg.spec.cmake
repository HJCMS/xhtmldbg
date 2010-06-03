####################################################################
## RPM Specfile template for xhtmldbg
## Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2009
## Copyright: See COPYING file that comes with this distribution
####################################################################

# norootforbuild

%define _qt_transdir  %(qmake -query QT_INSTALL_TRANSLATIONS)

Name:           xhtmldbg
Summary:        HTML/XHTML Debugger and Validator
Version:        @XHTMLDBG_PACKAGE_VERSION@
Release:        1
License:        GPLv3
AutoReqProv:    on
Group:          Productivity/Editors/Other
Url:            http://xhtmldbg.hjcms.de
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Requires:       QTidy >= @QTIDY_VERSION@ qtidyrc oxygen-icon-theme >= 4.4.0
BuildRequires:  cmake libtidy-devel QTidy-devel >= @QTIDY_VERSION@
BuildRequires:  update-desktop-files oxygen-icon-theme >= 4.4.0
BuildArch:      %{_target_cpu}
ExclusiveOs:    %{_os}

%debug_package

%description
Tidy HTML/XML Validator and Debugger.

Author:
-------------
  Juergen Heinemann (Undefined)

%package -n libxhtmldbg
Summary:     xhtmldbg Plugin Interface Library
License:     LGPLv3
Group:       System/Libraries
AutoReqProv: on

%description -n libxhtmldbg
This Library is used by xhtmldbg for load Plugins

Author:
-------------
  Juergen Heinemann (Undefined)

%package -n libxhtmldbg-devel
Summary:     Development Package for the xhtmldbg Plugin Interface Library
License:     LGPLv3
Group:       Development/Languages/C and C++
AutoReqProv: on
Requires:    %{name} >= %{version}
Requires:    libxhtmldbg = %{version}
Requires:    c++_compiler pkgconfig qt4-devel >= 4.6.0

%description -n libxhtmldbg-devel
This Library is used by xhtmldbg for load Plugins

Author:
-------------
  Juergen Heinemann (Undefined)

%prep

%setup -q -T -c %{name}

## Password: anonymous
git clone http://anonymous@repository.hjcms.de/hjcms/xhtmldbg .

%__mkdir_p build

%build

pushd build

cmake -Wno-dev \
  -DCMAKE_CXX_FLAGS:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_C_FLAGS:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_INSTALL_PREFIX:PATH=%{_prefix} \
  -DCMAKE_BUILD_TYPE:STRING=Debug \
  ../

%__make

popd

%install
## Reserved by Plugin Interface
%__mkdir_p $RPM_BUILD_ROOT/%{_libdir}/%{name}

pushd build
  %makeinstall
popd

%post
##

%postun
##

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/lib*.so
%{_qt_transdir}/xhtmldbg_*.qm
%dir %{_datadir}/%{name}
%doc %{_datadir}/%{name}/AUTHORS
%doc %{_datadir}/%{name}/COPYING
%doc %{_datadir}/%{name}/ChangeLog
%doc %{_datadir}/%{name}/NEWS
%doc %{_datadir}/%{name}/README
%{_datadir}/%{name}/xhtmldbg_untranslated.ts
%{_datadir}/applications/%{name}.desktop
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

%post -n libxhtmldbg -p /sbin/ldconfig
##

%postun -n libxhtmldbg -p /sbin/ldconfig
##

%files -n libxhtmldbg
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
%dir %{_datadir}/%{name}/plugintemplate
%{_datadir}/%{name}/plugintemplate/*.txt
%{_datadir}/%{name}/plugintemplate/*.h
%{_datadir}/%{name}/plugintemplate/*.cpp

%clean
test -d "$RPM_BUILD_ROOT" && rm -rf $RPM_BUILD_ROOT

%changelog
## LATEST @XHTMLDBG_VERSION@

