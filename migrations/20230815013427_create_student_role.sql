-- Insert 'administrator' role with Role_ID 1
INSERT INTO roles (Role_ID, Role_Name)
VALUES (1, 'administrator');

UPDATE users
SET role_id = 1
WHERE user_id = 2;

-- Update 'student' role's name
UPDATE roles
SET Role_Name = 'student'
WHERE Role_ID = 2;