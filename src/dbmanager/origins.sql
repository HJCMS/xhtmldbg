CREATE TABLE Origins (
  origin TEXT UNIQUE ON CONFLICT REPLACE,
  quota INTEGER NOT NULL ON CONFLICT FAIL
);