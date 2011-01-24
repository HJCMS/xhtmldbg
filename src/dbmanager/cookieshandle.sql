CREATE TABLE "cookieshandle" (
  "Hostname" TEXT  UNIQUE PRIMARY KEY ASC,
  "AccessType" INTEGER default 0,
  "AllowThirdParty" INTEGER default 0,
  "RFC2109" INTEGER default 1
);