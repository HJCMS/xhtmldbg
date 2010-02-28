####################################################################
## RPM Specfile template for xhtmldbg
## Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2009
## Copyright: See COPYING file that comes with this distribution
####################################################################

# norootforbuild

%if %{defined suse_version}
%define builddepends  libtidy-devel libQTidy-devel >= 0.8.2 libQtWebKit-devel >= 4.5.1 update-desktop-files oxygen-icon-theme >= 4.4.0
%else
%define builddepends  libtidy-devel libQTidy-devel >= 0.8.2 update-desktop-files oxygen-icon-theme >= 4.4.0
%endif

Name:           xhtmldbg
Summary:        HTML/XHTML Debugger and Validator
Version:        @XHTMLDBG_VERSION_STRING@
Release:        %(date +"%j")
License:        GPLv3
AutoReqProv:    on
Source0:        %{packagename}.tar.bz2
Group:          Productivity/Editors/Other
Url:            http://xhtmldbg.hjcms.de
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Requires:       libQTidy%{lt_version} = %{version} QTidy >= 0.7.1 qtidyrc
BuildRequires:  cmake %{builddepends}
BuildArch:      %{_target_cpu}
ExclusiveOs:    %{_os}

##% debug_package

%description
Tidy HTML/XML Validator and Debugger.

Author:
-------------
  Juergen Heinemann (Undefined)

%prep

%setup -q

%__mkdir_p build

%build

pushd build

cmake -Wno-dev \
  -DCMAKE_CXX_FLAGS:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_C_FLAGS:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_INSTALL_PREFIX:PATH=%{_qt_prefix} \
  -DCMAKE_BUILD_TYPE:STRING=Debug \
  ../

%__make

popd

%install

pushd build
  %makeinstall
popd

## Update Desktop Files
%if %{defined suse_version}
%suse_update_desktop_file -i %{name}  System Utility
%endif 

%post
##

%postun
##

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_qt_transdir}/qtidy_*.qm
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
%{_datadir}/icons/oxygen/128x128/apps/%{name}.png
%dir %{_datadir}/icons/oxygen/16x16
%dir %{_datadir}/icons/oxygen/16x16/apps
%{_datadir}/icons/oxygen/16x16/apps/%{name}.png
%dir %{_datadir}/icons/oxygen/192x192
%dir %{_datadir}/icons/oxygen/192x192/apps
%{_datadir}/icons/oxygen/192x192/apps/%{name}.png
%dir %{_datadir}/icons/oxygen/22x22
%dir %{_datadir}/icons/oxygen/22x22/apps
%{_datadir}/icons/oxygen/22x22/apps/%{name}.png
%dir %{_datadir}/icons/oxygen/24x24
%dir %{_datadir}/icons/oxygen/24x24/apps
%{_datadir}/icons/oxygen/24x24/apps/%{name}.png
%dir %{_datadir}/icons/oxygen/32x32
%dir %{_datadir}/icons/oxygen/32x32/apps
%{_datadir}/icons/oxygen/32x32/apps/%{name}.png
%dir %{_datadir}/icons/oxygen/36x36
%dir %{_datadir}/icons/oxygen/36x36/apps
%{_datadir}/icons/oxygen/36x36/apps/%{name}.png
%dir %{_datadir}/icons/oxygen/48x48
%dir %{_datadir}/icons/oxygen/48x48/apps
%{_datadir}/icons/oxygen/48x48/apps/%{name}.png
%dir %{_datadir}/icons/oxygen/64x64
%dir %{_datadir}/icons/oxygen/64x64/apps
%{_datadir}/icons/oxygen/64x64/apps/%{name}.png
%dir %{_datadir}/icons/oxygen/72x72
%dir %{_datadir}/icons/oxygen/72x72/apps
%{_datadir}/icons/oxygen/72x72/apps/%{name}.png
%dir %{_datadir}/icons/oxygen/96x96
%dir %{_datadir}/icons/oxygen/96x96/apps
%{_datadir}/icons/oxygen/96x96/apps/%{name}.png
%{_datadir}/pixmaps/%{name}.xpm

%clean
test -d "$RPM_BUILD_ROOT" && rm -rf $RPM_BUILD_ROOT

%changelog
## LATEST @XHTMLDBG_VERSION_STRING@

