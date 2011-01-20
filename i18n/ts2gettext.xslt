<?xml version = '1.0' encoding = 'utf-8' ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
   xmlns:date="http://exslt.org/dates-and-times"
   xmlns:str="http://exslt.org/strings"
   extension-element-prefixes="date str"
>

<!--
  This file is part of the xhtmldbg project

  Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2010

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
-->

<xsl:output version="1.0"
  method="text"
  encoding="utf-8"
  media-type="text/plain" />

<xsl:variable name="version" value="0.8.11.rc1" />
<xsl:variable name="email" value="nospam@hjcms.de" />

<!-- Hide -->
<xsl:template match="name" />

<!-- Source Location Comment -->
<xsl:template match="location" mode="source">
<xsl:value-of select="str:replace(@filename,'../',' ')" />:<xsl:value-of select="@line" />
</xsl:template>

<!-- Message Block 
{http://icl.com/saxon}expression
-->
<xsl:template match="message">
#:<xsl:apply-templates select="location" mode="source" />
msgid  "<xsl:value-of select="str:replace(normalize-space(source),'&quot;','\&quot;')" />"
msgstr "<xsl:value-of select="str:replace(normalize-space(translation),'&quot;','\&quot;')" />"
</xsl:template>

<!-- Main -->
<xsl:template match="/TS">
<xsl:text># SOME DESCRIPTIVE TITLE.
# Copyright (C) YEAR GPLv3
# This file is distributed under the same license as the PACKAGE package.
# FIRST AUTHOR &lt;EMAIL@ADDRESS&gt;, YEAR.
#
#, fuzzy
msgid ""
msgstr ""
"Project-Id-Version: xhtmldbg </xsl:text><xsl:value-of select="$version" /><xsl:text>\n"
"Report-Msgid-Bugs-To: </xsl:text><xsl:value-of select="$email" /><xsl:text>\n"
"POT-Creation-Date: </xsl:text><xsl:value-of select="date:date-time()" /><xsl:text>\n"
"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\n"
"Last-Translator: FULL NAME &lt;EMAIL@ADDRESS&gt;\n"
"Language-Team: LANGUAGE &lt;LL@li.org&gt;\n"
"MIME-Version: 1.0\n"
"Content-Type: text/plain; charset=UTF-8\n"
"Content-Transfer-Encoding: 8bit\n"
</xsl:text>
<xsl:apply-templates select="./context/*" />
</xsl:template>
</xsl:stylesheet>
