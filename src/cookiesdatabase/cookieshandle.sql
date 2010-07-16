CREATE TABLE "cookieshandle" (
  "AccessType" INTEGER PRIMARY KEY ASC default 0,
  "Hostname" TEXT default '',
  "AllowThirdParty" INTEGER default 0,
  "RFC2109" INTEGER default 1
);
