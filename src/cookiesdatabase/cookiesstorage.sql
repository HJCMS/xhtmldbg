CREATE TABLE "cookiesstorage" (
  "domain" TEXT UNIQUE PRIMARY KEY ASC,
  "name" TEXT,
  "expirationDate" TEXT,
  "data" TEXT
);
