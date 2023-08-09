#!/usr/bin/env bash
set -x
set -eo pipefail

if ! [ -x "$(command -v psql)" ]; then
    echo >&2 "Error: psql is not installed."
    echo >&2 "Please make sure PostgreSQL is installed and added to the system PATH."
    exit 1
fi

if ! [ -x "$(command -v sqlx)" ]; then
    echo >&2 "Error: sqlx is not installed."
    echo >&2 "Please install sqlx CLI using 'cargo install --version=0.5.7 sqlx-cli --no-default-features --features postgres'."
    exit 1
fi

DB_USER=${POSTGRES_USER:=postgres}
DB_PASSWORD="${POSTGRES_PASSWORD:=password}"
DB_NAME="${POSTGRES_DB:=coursesyncdb}"
DB_HOST="${POSTGRES_HOST:=localhost}"
DB_PORT="${POSTGRES_PORT:=5432}"

export PGPASSWORD="${DB_PASSWORD}"

>&2 echo "Waiting for PostgreSQL to start..."

until psql -h "${DB_HOST}" -U "${DB_USER}" -p "${DB_PORT}" -c '\q' >/dev/null 2>&1; do
    >&2 echo "PostgreSQL is still unavailable - sleeping"
    sleep 1
done

>&2 echo "PostgreSQL is up and running on host ${DB_HOST}, port ${DB_PORT} - running migrations now!"

if ! psql -h "${DB_HOST}" -U "${DB_USER}" -p "${DB_PORT}" -lqt | cut -d \| -f 1 | grep -qw "${DB_NAME}"; then
    >&2 echo "Database ${DB_NAME} does not exist. Creating it..."
    createdb -h "${DB_HOST}" -U "${DB_USER}" -p "${DB_PORT}" "${DB_NAME}"
fi

export DATABASE_URL=postgres://${DB_USER}:${DB_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}

cd ..
# sqlx migrate add create_login_timestamps_table
sqlx migrate run

>&2 echo "PostgreSQL has been migrated, ready to go!"
