-- Login Timestamps table
CREATE TABLE login_timestamps (
    Login_ID SERIAL PRIMARY KEY,
    User_ID INT,
    Login_Timestamp TIMESTAMP
);
