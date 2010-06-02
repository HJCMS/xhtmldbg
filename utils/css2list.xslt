<?xml version = '1.0' encoding = 'utf-8' ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
   xmlns:exsl="http://exslt.org/common"
   extension-element-prefixes="exsl"
   execlude-result-prefixes="exsl">

<xsl:output version="1.0"
  method="text"
  encoding="utf-8"
  media-type="text/plain"
/>

<xsl:template match="code">
<xsl:if test="string-length(text()) &gt; 3">
  <xsl:value-of select="." /><xsl:text>
</xsl:text>
</xsl:if>
</xsl:template>

<xsl:template match="h3">
</xsl:template>

<xsl:template match="a">
</xsl:template>

<xsl:template match="span">
</xsl:template>

<xsl:template match="/html/head">
</xsl:template>

<xsl:template match="/html/body">
  <xsl:apply-templates match="ul/*" />
</xsl:template>

</xsl:stylesheet>
