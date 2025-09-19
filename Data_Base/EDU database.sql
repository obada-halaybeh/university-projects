create database EDU;
use EDU;

CREATE TABLE donor(
    donor_id INT PRIMARY KEY,
    first_name VARCHAR(50) NULL,
    last_name VARCHAR(50) NULL,
    type VARCHAR(50) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    staff_id INT
);

ALTER TABLE donor ADD FOREIGN KEY(staff_id)REFERENCES staff(staff_id) ON UPDATE CASCADE ON DELETE CASCADE;

INSERT INTO donor(donor_id, first_name, last_name, type, email, staff_id)
VALUES
    (1, 'Michael', 'Brown', 'Individual', 'Michael.Brown@gmail.com', 1),
    (2, 'Eva', 'Davis', 'Individual', 'Eva.Davis@gmail.com', 1),
    (3, 'David', 'Wilson', 'Individual', 'David.Wilson@gmail.com', 1),
    (4, 'Sophia', 'Martinez', 'Individual', 'Sophia.Martinez@gmail.com', 4),
    (5, 'James', 'Anderson', 'Individual', 'James.Anderson@gmail.com', 4),
    (6, 'Olivia', 'Thomas', 'Individual', 'Olivia.Thomas@gmail.com', 5),
    (7, NULL, NULL, 'organization', 'steev.organization@gmail.com', 2),
    (8, NULL, NULL, 'organization', 'brox.organization@gmail.com', 2),
    (9, NULL, NULL, 'organization', 'fam.organization@gmail.com', 3),
    (10, NULL, NULL, 'organization', 'dov.organization@gmail.com', 3);

SELECT * FROM donor WHERE type = 'Individual';
SELECT * FROM donor WHERE type = 'organization';
SELECT * FROM donor;

CREATE TABLE donations(
    donation_id INT PRIMARY KEY,
    amount DECIMAL(10,2) NOT NULL,
    donation_date DATE NOT NULL,
    donor_id INT,
    event_id INT
);

ALTER TABLE donations ADD FOREIGN KEY(donor_id)REFERENCES donor(donor_id) ON UPDATE CASCADE ON DELETE CASCADE;
ALTER TABLE donations ADD FOREIGN KEY(event_id)REFERENCES events(event_id) ON UPDATE CASCADE ON DELETE CASCADE;

INSERT INTO donations(donation_id, amount, donation_date, donor_id, event_id)
VALUES
    (1, 1200, '2025-1-1', 1, 1),
    (2, 500, '2025-1-2', 1, 1),
    (3, 1000, '2025-1-2', 7, 1),
    (4, 750, '2025-2-16', 2, 2),
    (5, 250, '2025-2-16', 4, 2),
    (6, 5000, '2025-2-17', 10, 2),
    (7, 500, '2025-6-20', 2, 3),
    (8, 250, '2025-6-20', 3, 3),
    (9, 300, '2025-6-20', 3, 3),
    (10, 1000, '2025-4-15', 4, 4),
    (11, 2500, '2025-4-16', 9, 4),
    (12, 350, '2025-4-16', 6, 4),
    (13, 1100, '2025-11-1', 5, 5),
    (14, 400, '2025-11-2', 5, 5),
    (15, 1000, '2025-11-3', 8, 5),
    (16, 500, '2025-5-2', 6, 6),
    (17, 700, '2025-5-4', 8, 6),
    (18, 200, '2025-5-9', 10, 6),
    (19, 500, '2025-5-15', 1, 6),
    (20, 1000, '2025-5-20', 7, 6),
    (21, 200, '2025-6-1', 9, 6),
    (22, 400, '2025-8-1', 2, 7),
    (23, 1250, '2025-8-4', 5, 7),
    (24, 100, '2025-8-5', 3, 7),
    (25, 200, '2025-8-9', 8, 7);

SELECT * FROM donations;
SELECT * FROM donations WHERE event_id = 7;

CREATE TABLE events(
    event_id INT PRIMARY KEY,
    event_name VARCHAR(50) NOT NULL,
    start_date DATE NOT NULL,
    end_date DATE NOT NULL
);


INSERT INTO events(event_id, event_name, start_date, end_date)
VALUES
    (1, 'Winter Festival', '2025-1-1', '2025-1-2'),
    (2, 'School of Hop', '2025-2-15', '2025-2-17'),
    (3, 'Summer Festival', '2025-6-20', '2025-6-22'),
    (4, 'Spring Festival', '2025-4-15', '2025-4-17'),
    (5, 'Fall Festival', '2025-11-1', '2025-11-3'),
    (6, 'Country Run', '2025-5-1', '2025-6-1'),
    (7, 'Down Town', '2025-8-1', '2025-8-10');

CREATE TABLE staff(
    staff_id INT PRIMARY KEY,
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    role VARCHAR(50) NOT NULL DEFAULT 'employee',
    email VARCHAR(100) UNIQUE NOT NULL,
    annual_salary DECIMAL(10, 2) NOT NULL
);


INSERT INTO staff (staff_id, first_name, last_name, role, email, annual_salary)
VALUES
    (1, 'Issac', 'Hubbard', DEFAULT, 'issac.hubbard@gmail.com', 50000.00),
    (2, 'Luna', 'Snow', 'manager', 'luna.snow@gmail.com', 75000.00),
    (3, 'Kayla', 'Frost', 'manager', 'Kayla.Frost@gmail.com', 80000.00),
    (4, 'Frank', 'Miller', DEFAULT, 'Frank.Miller@gmail.com', 45000.00),
    (5, 'Grace', 'Wilson', DEFAULT, 'Grace.Wilson@gmail.com', 30000.00);

SELECT * FROM staff WHERE role = 'employee';
SELECT * FROM staff WHERE role = 'manager';
SELECT * FROM staff;
                  

create table donor_address(
donor_id int,
state varchar(50),
city varchar(50),
street varchar(50),
primary key (donor_id, state, city, street)
);

ALTER TABLE donor_address ADD FOREIGN KEY(donor_id)REFERENCES donor(donor_id) ON UPDATE CASCADE ON DELETE CASCADE;

insert into donor_address (donor_id, state, city, street)
values					  (1, 'New York'  , 'Delmar', '1037 Hardesty Street'),
						  (1, 'Florida'  , 'Pompano Beach', '1575 Foley Street'),
                          (2, 'Kentucky'  , 'Lexington', '120 North Bend River Road'),
                          (3, 'Maryland'  , 'Columbia', '4062 Hewes Avenue'),
                          (4, 'North Carolina'  , 'Raleigh', '4122 Johnson Street'),
                          (4, 'Virginia'  , 'Herndon', '746 Sunrise Road'),
                          (5, 'Oregon'  , 'Portland', '3288 Mattson Street'),
                          (5, 'Indiana'  , 'Evansville', '1980 Stratford Park'),
                          (6, 'Michigan'  , 'Grand Rapids', '171 Echo Lane'),
                          (7, 'California'  , 'Fremont', '1617 Ella Street'),
                          (8, 'New Jersey'  , 'New Brunswick', '1889 Williams Mine Road'),
                          (9, 'New York'  , 'Albany', '2611 Golden Ridge Road'),
                          (10, 'Pennsylvania'  , 'Philadelphia', '2577 Berkley Street');
                          
                          
select * from donor_address where donor_id = 1;
  select * from donor_address;                        
						  

create table staff_phone (
staff_id int,
phone varchar(15),
primary key (staff_id, phone)
);


ALTER TABLE staff_phone ADD FOREIGN KEY(staff_id)REFERENCES staff(staff_id) ON UPDATE CASCADE ON DELETE CASCADE;

insert into staff_phone (staff_id, phone)
values					(1     , '+1 305-241-8749'),
						(1     , '+1 719-200-4137'),
						(2     , '+1 503-341-4061'),
						(3     , '+1 906-458-5993'),
						(4     , '+1 660-247-0609'),
						(4     , '+1 361-462-3441'),
						(5     , '+1 252-282-0855');
						

create table donor_phone (
donor_id int,
phone varchar(15),
primary key (donor_id, phone)
);


ALTER TABLE donor_phone ADD FOREIGN KEY(donor_id)REFERENCES donor(donor_id) ON UPDATE CASCADE ON DELETE CASCADE;

insert into donor_phone (donor_id, phone)
values					  (1     , '+1 845-200-9047'),
						  (1     , '+1 561-929-1270'),
                          (2     , '+1 502-488-2827'),
                          (3     , '+1 919-809-5336'),
                          (4     , '+1 703-282-3635'),
                          (4     , '+1 503-515-6733'),
                          (5     , '+1 812-305-1237'),
                          (5     , '+1 616-478-1457'),
                          (6     , '+1 267-752-9808'),
                          (7     , '+1 817-320-3013'),
                          (8     , '+1 508-973-3274'),
                          (9     , '+1 502-802-8790'),
                          (10     , '+1 813-765-7392');
select * from donor_phone where donor_id = 1;

create table event_location (
event_id int,
location varchar(100),
primary key (event_id, location)
);

ALTER TABLE event_location ADD FOREIGN KEY(event_id)REFERENCES events(event_id) ON UPDATE CASCADE ON DELETE CASCADE;

insert into event_location (event_id, location)
values 					   (1, 'Central Park, NY'),
						   (2, 'Broadway Theater'),
						   (3, 'Miami Beach, FL'),
   						   (4, 'Chicago Botanic Garden'),
 						   (5, 'Denver City Park'),
   						   (6, 'Route 66'),
                           (6, 'Yellowstone Park'),
						   (6, 'Grand Canyon'),
                           (6, 'Washington Monument'),
						   (6, 'Blue Ridge Parkway'),
   						   (7, 'Los Angeles, CA'),
                           (7, 'San Diego, CA'),
						   (7, 'San Francisco, CA');
                           
select * from event_location;

create table donor_event (
donor_id int,
event_id int,
primary key (donor_id, event_id)
);

ALTER TABLE donor_event ADD FOREIGN KEY(donor_id)REFERENCES donor(donor_id) ON UPDATE CASCADE ON DELETE CASCADE;
ALTER TABLE donor_event ADD FOREIGN KEY(event_id)REFERENCES events(event_id) ON UPDATE CASCADE ON DELETE CASCADE;

insert into donor_event(donor_id, event_id)
values				   (1,1),
					   (1,6),
                       (1,7),
                       (1,4),
                       (2,2),
                       (2,3),
                       (2,6),
                       (2,7),
                       (3,1),
                       (3,3),
                       (3,6),
                       (3,7),
                       (4,2),
                       (4,4),
                       (4,5),
                       (4,6),
                       (4,7),
                       (5,3),
                       (5,4),
                       (5,7),
                       (6,4),
                       (6,6),
                       (7,1),
                       (7,2),
                       (7,3),
                       (7,4),
                       (7,5),
                       (7,6),
                       (7,7),
                       (8,1),
                       (8,2),
                       (8,3),
                       (8,4),
                       (8,5),
                       (8,6),
                       (8,7),
                       (9,1),
                       (9,2),
                       (9,3),
                       (9,4),
                       (9,5),
                       (9,6),
                       (9,7),
                       (10,1),
                       (10,2),
                       (10,3),
                       (10,4),
                       (10,5),
                       (10,6),
                       (10,7);
						
SELECT * FROM donor_event where event_id= 2;
SELECT * FROM donor_event;

create table staff_event (
staff_id int,
event_id int,
primary key (staff_id, event_id)
);

ALTER TABLE staff_event ADD FOREIGN KEY(staff_id)REFERENCES staff(staff_id) ON UPDATE CASCADE ON DELETE CASCADE;
ALTER TABLE staff_event ADD FOREIGN KEY(event_id)REFERENCES events(event_id) ON UPDATE CASCADE ON DELETE CASCADE;

insert into staff_event(staff_id, event_id)
values				   (1,1),
					   (2,1),
                       (3,2),
                       (4,2),
                       (5,3),
                       (2,3),
                       (3,4),
                       (1,4),
                       (1,5),
                       (3,5),
                       (5,6),
                       (2,6),
                       (3,7),
                       (4,7);
                       
select * from staff_event;

-- Views
-- 1 view
SELECT d.first_name, d.last_name
FROM donor AS d;

create view donor_contribution as
select d.donor_id, concat(d.first_name,' ' ,d.last_name) as full_name,d.type, d.email, d.staff_id, sum(da.amount) as total_donations, count(da.donation_id) as donation_count 
from donor d
left join donations da
on d.donor_id = da.donor_id
group by d.donor_id;

select * from donor_contribution order by full_name ASC;

-- 2 view
create view event_donations as
select e.event_name, e.start_date, e.end_date, el.location, SUM(da.amount) AS total_donations
from events e
left join 
event_location el
on 
e.event_id = el.event_id
left join 
donations da
on 
e.event_id = da.event_id
group by 
    e.event_id, e.event_name, e.start_date, e.end_date, el.location;
select * from event_donations;    

-- 3 view
create view donor_contact_information as
select CONCAT(d.first_name, ' ', d.last_name) AS full_name, d.email, da.state, da.city, da.street, dp.phone
from
donor d
left join
donor_address da
on
d.donor_id = da.donor_id
left join
donor_phone dp
on 
d.donor_id = dp.donor_id;

select * from donor_contact_information;

-- 4 view
create view staff_contribution_report as
select  s.staff_id, CONCAT(s.first_name, ' ', s.last_name) as full_name, s.role, s.email, COUNT(distinct d.donor_id) as donor_count, SUM(da.amount) as total_donations
from Staff s
left join
donor d
on
s.staff_id = d.staff_id
left join 
donations da
on 
d.donor_id = da.donor_id
group by 
s.staff_id, s.first_name, s.last_name, s.role, s.email;

select * from staff_contribution_report;

-- proceduers
-- proceduer 1
DELIMITER //

CREATE PROCEDURE AddDonor(IN d_id INT, IN f_name VARCHAR(50), IN l_name VARCHAR(50), IN donor_type VARCHAR(50), IN donor_email VARCHAR(100), IN s_id INT)
BEGIN
    INSERT INTO donor (donor_id, first_name, last_name, type, email, staff_id)
    VALUES (d_id,f_name, l_name, donor_type, donor_email, s_id);
END //

DELIMITER ;

-- proceduer 2
DELIMITER ##

CREATE PROCEDURE UpdateDonorEmail(IN donor_id INT, IN new_email VARCHAR(100))
BEGIN
    UPDATE donor SET email = new_email WHERE donor_id = donor_id;
END ##

DELIMITER ;

-- proceduer 3
DELIMITER $$

CREATE PROCEDURE AddDonation(IN donation_amount DECIMAL(10,2), IN donation_date DATE, IN donor_id INT, IN event_id INT)
BEGIN
    INSERT INTO donations (amount, donation_date, donor_id, event_id)
    VALUES (donation_amount, donation_date, donor_id, event_id);
END $$

DELIMITER ;

-- PROCEDURE 4
DELIMITER //

CREATE PROCEDURE AssignStaffToEvent(IN staff_id INT, IN event_id INT)
BEGIN
    INSERT INTO staff_event (staff_id, event_id)
    VALUES (staff_id, event_id);
END //

DELIMITER ;

-- user creating
SHOW GRANTS FOR admin;
select * from mysql.user;

CREATE USER admin IDENTIFIED BY 'admin';

GRANT ALL PRIVILEGES ON edu.* TO admin;

CREATE USER staff IDENTIFIED BY 'staff';

GRANT SELECT, UPDATE ON edu.donor TO staff;
GRANT SELECT, UPDATE ON edu.donations TO staff;
GRANT SELECT, UPDATE ON edu.donor_address TO staff;
GRANT SELECT, UPDATE ON edu.donor_phone TO staff;


CREATE USER donor IDENTIFIED BY 'donor';

GRANT SELECT ON edu.donations TO donor;
GRANT SELECT ON edu.donor_phone TO donor;
GRANT SELECT ON edu.donor_address TO donor;

show grants for donor;

-- testing

-- PK
INSERT INTO donor (donor_id, first_name, last_name, type, email, staff_id)
VALUES (1, 'Test', 'User', 'Individual', 'test.user@gmail.com', 1);

INSERT INTO donor (donor_id, first_name, last_name, type, email, staff_id)
VALUES (NULL, 'Test', 'User', 'Individual', 'test.user2@gmail.com', 1);

-- FK
INSERT INTO donor (donor_id, first_name, last_name, type, email, staff_id)
VALUES (11, 'Test', 'User', 'Individual', 'test.user@gmail.com', 999);

DELETE FROM staff WHERE staff_id = 1;

ALTER TABLE donor DROP FOREIGN KEY donor_ibfk_1;
ALTER TABLE donor ADD FOREIGN KEY (staff_id) REFERENCES staff(staff_id) ON DELETE RESTRICT;
DELETE FROM staff WHERE staff_id = 2;

-- unique

INSERT INTO donor (donor_id, first_name, last_name, type, email, staff_id)
VALUES (11, 'Test', 'User', 'Individual', 'James.Anderson@gmail.com', 2);

-- default

INSERT INTO staff (staff_id, first_name, last_name, email, annual_salary)
VALUES (6, 'Test', 'Staff', 'test.staff@gmail.com', 40000.00);
SELECT * FROM staff where staff_id = 6;

-- not null

INSERT INTO donor (donor_id, first_name, last_name, type, email, staff_id)
VALUES (12, 'Test', 'User', NULL, 'test.user3@gmail.com', 3);


-- select
select * from donor;
select * from donor where type = 'organization';

insert into staff (staff_id, first_name, last_name, email, annual_salary)
values (7, 'omar','alhalaybeh', 'omar.al@gmail.com', 450000.00);
select * from staff;

call AddDonor(11,'max', 'wilyam', 'Individual','max.wilyam@gmail.com', 3);
select * from donor;



-- gui test

select * from donations;