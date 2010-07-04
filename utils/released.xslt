<?xml version = '1.0' encoding = 'utf-8' ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output version="1.0" method="text" encoding="utf-8" media-type="text/plain" />

<xsl:template match="/version">
<xsl:value-of select="date" />
</xsl:template>

</xsl:stylesheet>
