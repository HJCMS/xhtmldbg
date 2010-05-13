<?xml version = '1.0' encoding = 'utf-8' ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output version="1.0" method="text"  encoding="utf-8" media-type="text/plain" />

<xsl:template match="location" />

<xsl:template match="message">
#<xsl:value-of select="preceding-sibling::name" />
  SOURCE       : <xsl:value-of select="source" />
<xsl:text>
</xsl:text>
  TRANSLATION  : <xsl:value-of select="translation" />
<xsl:text>

</xsl:text>
</xsl:template>

<xsl:template match="/TS">
  <xsl:apply-templates select="./context/*" />
</xsl:template>

</xsl:stylesheet>