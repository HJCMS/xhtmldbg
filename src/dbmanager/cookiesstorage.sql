CREATE TABLE "cookiesstorage" (
  "domain" TEXT,
  "path" TEXT default '/',
  "name" TEXT,
  "expiration" INTEGER,
  "data" TEXT
);