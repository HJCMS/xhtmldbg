[General]
Version="@XHTMLDBG_VERSION@"
AutoLoadImages=true
PluginsEnabled=false
HideDownloadManager=true
DefaultFontSize=16
DefaultFixedFontSize=14
css_lang=en
css_medium=all
css_warning=2
css_appl=@JAVA_RUNTIME@
enableHighlightBackground=true
enableHighlightBorder=false
enableProxy=false
sslCaCertsDatabase=@CA_BUNDLE_FILE@
icontheme=oxygen
iconthemepaths=@ICON_THEME_PATH@
GeoIP_Database=@GEOIP_DATABASE_PATH@/GeoIP.dat
webkit_plugin_path=@CMAKE_INSTALL_PREFIX@/lib@LIB_SUFFIX@/xhtmldbg/browser-plugins

[HeaderDefinitions]
Accept-Charset="utf-8, unicode-1-1;q=0.8"
Accept="application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5"

[TrustedCertsHosts]
1\host=hjcms.de
size=1

[Qt]
QtWebKit\QWebInspector\resourceTrackingEnabled=true
QtWebKit\QWebInspector\resourceTrackingEnabled.type=QString
QtWebKit\QWebInspector\debuggerEnabled=true
QtWebKit\QWebInspector\debuggerEnabled.type=bool
QtWebKit\QWebInspector\profilerEnabled=true
QtWebKit\QWebInspector\profilerEnabled.type=QString
QtWebKit\QWebInspector\panelEnablerView=true
QtWebKit\QWebInspector\panelEnablerView.type=bool
QtWebKit\QWebInspector\resourceTrackingAlwaysEnabled=true
QtWebKit\QWebInspector\resourceTrackingAlwaysEnabled.type=bool
QtWebKit\QWebInspector\debuggerAlwaysEnabled=true
QtWebKit\QWebInspector\debuggerAlwaysEnabled.type=bool
QtWebKit\QWebInspector\profilerAlwaysEnabled=true
QtWebKit\QWebInspector\profilerAlwaysEnabled.type=bool
QtWebKit\QWebInspector\auditsPanelEnabled=true
QtWebKit\QWebInspector\auditsPanelEnabled.type=bool

[UserAgents]
1\name=XHTMLDBG(DE)
1\agent=Mozilla/5.0
1\tokens="(compatible; XHTMLDBG; de, de_DE; X11) AppleWebKit (KHTML, like Gecko)"
2\name=XHTMLDBG(EN)
2\agent=Mozilla/5.0
2\tokens="(compatible; XHTMLDBG; en, en_US; X11) AppleWebKit (KHTML, like Gecko)"
size=2

[Networking]
CachingEnabled=false
ModifyCachingBehavior=false

[WebDatabase]
DefaultQuota=13346

[HTML5SecurityOptions]
size=1
1\host=xhtmldbg.hjcms.de
1\port=80
1\scheme=http

[Plugins]
SelfHtmlSidebarUrl=http://de.selfhtml.org/navigation/sidebars/html.htm

[ApacheLoggerPlugin]
directories=/var/log/apache2
scandir=/var/log/apache2
logs=/var/log/apache2/error_log, /var/log/apache2/rewrite.log

[Phonon::AudioOutput]
xhtmldbg_Volume=0.50

[Kate Document Defaults]
Allow End of Line Detection=true
Allow Simple Mode=true
Show Tabs=true
Tab Handling=2
Tab Width=4
Wrap Cursor=true

[Kate View Defaults]
Allow Mark Menu=true
Folding Bar=true
Icon Bar=true
Line Numbers=true
