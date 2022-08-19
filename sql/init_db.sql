-- Create film photo database data model in a db

-- drop cascade schema metadata_schema;
create schema if not exists metadata_schema
  authorization film_photo_db_owner;

-- drop cascade schema objects_schema;
create schema if not exists objects_schema
  authorization film_photo_db_owner;

create table if not exists metadata_schema.entity_type (
  entity_type_id bigserial primary key,
  digital bool,
  max_instances integer,
  name text,
  description text,
  extended_props text[],
  inherited_props text
);

create table if not exists metadata_schema.lab (
  lab_id bigserial primary key,
  name text,
  description text,
  street_address text,
  web_address text,
  email_address text,
  phone_customer_service text
);

create table if not exists metadata_schema.process (
  process_id bigserial primary key,
  name text,
  description text
);

create table if not exists metadata_schema.assignment (
  assignment_id bigserial primary key,
  name text,
  description text,
  location text,
  start_date date,
  end_date date
);

create table if not exists metadata_schema.camera (
  camera_id bigserial primary key,
  name text,
  description text,
  manufacturer text,
  model text,
  lens_removeable bool
);

create table if not exists metadata_schema.lens (
  lens_id bigserial primary key,
  name text,
  description text,
  manufacturer text,
  model text,
  built_in bool,
  f_number text,
  focal_length text
);

create table if not exists metadata_schema.film (
  film_id bigserial primary key,
  name text,
  description text,
  manufacturer text,
  color bool,
  negative bool,
  listed_process text,
  iso integer,
  listed_ei integer[]
);

create table if not exists objects_schema.roll (
  roll_id bigserial primary key,
  name text,
  description text,
  lab_id bigint references metadata_schema.lab,
  process_id bigint references metadata_schema.process,
  developed_date date,
  old_roll_ident text,
  lab_roll_ident text
);

create table if not exists objects_schema.frame (
  frame_id bigserial primary key,
  roll_id bigint references objects_schema.roll,
  frame_num integer,
  name text,
  description text,
  date_time_stamp timestamptz,
  assignment_id bigint references metadata_schema.assignment,
  camera_id bigint references metadata_schema.camera,
  lens_id bigint references metadata_schema.lens,
  film_id bigint references metadata_schema.film,
  ei bigint
);

create table if not exists objects_schema.entity (
  entity_id bigserial primary key,
  frame_id bigint references objects_schema.frame,
  entity_type_id bigint references metadata_schema.entity_type,
  name text,
  description text,
  date_time_stamp timestamptz,
  series text,
  extended_props text[]
);
