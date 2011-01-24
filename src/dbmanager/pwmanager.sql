CREATE TABLE "pwmanager" (
  "Url" TEXT  UNIQUE PRIMARY KEY ASC,
  "Realm" TEXT,
  "Pass" TEXT,
  "Host" TEXT
);
