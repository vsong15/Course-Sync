-- Insert the 'administrator' role if it doesn't exist
INSERT INTO roles (role_name)
SELECT 'administrator'
WHERE NOT EXISTS (
    SELECT 1 FROM roles WHERE role_name = 'administrator'
);

INSERT INTO users (username, password, role_id, first_name, last_name, email)
SELECT 'admin', 'admin123', role_id, 'System', 'Administrator', 'admin@example.com'
FROM roles
WHERE role_name = 'administrator'
AND NOT EXISTS (
    SELECT 1 FROM users WHERE username = 'admin'
);
