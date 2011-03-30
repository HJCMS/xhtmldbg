CREATE TABLE formmanager (
  url TEXT UNIQUE NOT NULL ON CONFLICT FAIL,
  project TEXT NOT NULL DEFAULT 'Form 0',
  autoinsert INTEGER default 0
);