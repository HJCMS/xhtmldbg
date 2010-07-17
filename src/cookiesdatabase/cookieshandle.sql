CREATE TABLE "cookieshandle" (
  "Hostname" TEXT PRIMARY KEY ASC default '',
  "AccessType" INTEGER default 0,
  "AllowThirdParty" INTEGER default 0,
  "RFC2109" INTEGER default 1
);
