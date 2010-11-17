## file:///xhtmldbg.ebuild
# Copyright 1999-2010 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=2

inherit cmake-utils 

DESCRIPTION="xhtmldbg is a XHTML / HTML Debugger for Website Developers. "
HOMEPAGE="http://xhtmldbg.hjcms.de"
SRC_URI="ftp://ftp.hjcms.de/xhtmldbg/${P}/${P}.tar.bz2"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="~x86"
IUSE="debug doc"

DEPEND=">=x11-libs/qt-core-4.7.0
		>=x11-libs/qt-gui-4.7.0
		>=app-text/htmltidy-20090325
		>=dev-util/cmake-2.8.0
		>=dev-libs/libQTidy-0.8.2
		net-dns/bind-tools
		>=kde-base/oxygen-icons-4.4.0
		>=media-libs/raptor-1.4.19
		>=dev-libs/geoip-1.4.5
		>=dev-libs/nss-3.12.8
		>=dev-libs/nspr-4.8.6
		>=dev-libs/xulrunner-1.9.2.12"
RDEPEND="${DEPEND}
		doc? ( app-doc/doxygen )"

src_configure() {
	cmake-utils_src_configure
}

src_compile() {
	cmake-utils_src_compile
}

src_install() {
	cmake-utils_src_install
}
