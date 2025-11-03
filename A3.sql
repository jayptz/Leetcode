-- reset & use
DROP SCHEMA IF EXISTS eticket_reservation;
CREATE SCHEMA eticket_reservation DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci;
USE eticket_reservation;

-- 1. UserAccount
CREATE TABLE IF NOT EXISTS UserAccount (
  accountID   INT NOT NULL,
  email       VARCHAR(100) NOT NULL,
  password    VARCHAR(100) NOT NULL,
  PRIMARY KEY (accountID),
  UNIQUE (email)
) ENGINE=InnoDB;

-- 2. Customer (FK -> UserAccount)
CREATE TABLE IF NOT EXISTS Customer (
  accountID  INT NOT NULL,
  name       VARCHAR(100) NOT NULL,
  DOB        DATE NULL,
  SIN        CHAR(9) NULL,
  PRIMARY KEY (accountID),
  UNIQUE (SIN),
  FOREIGN KEY (accountID) REFERENCES UserAccount(accountID)
    ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- 3. CustomerPhone (FK -> Customer)
CREATE TABLE IF NOT EXISTS CustomerPhone (
  accountID     INT NOT NULL,
  phone_number  VARCHAR(25) NOT NULL,
  label         ENUM('Mobile','Home','Work','Other') DEFAULT 'Mobile',
  PRIMARY KEY (accountID, phone_number),
  FOREIGN KEY (accountID) REFERENCES Customer(accountID)
    ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- 4. Route
CREATE TABLE IF NOT EXISTS Route (
  routeID      INT NOT NULL,
  code         VARCHAR(16) NOT NULL,
  name         VARCHAR(100) NOT NULL,
  distance_km  DECIMAL(8,2) NOT NULL,
  duration_min INT NOT NULL,
  PRIMARY KEY (routeID),
  UNIQUE (code),
  CHECK (distance_km >= 0),
  CHECK (duration_min > 0)
) ENGINE=InnoDB;

-- 5. Vehicle
CREATE TABLE IF NOT EXISTS Vehicle (
  vehicleID    INT NOT NULL,
  vehicle_type ENUM('Bus','Train','Aircraft','Other') NOT NULL,
  model        VARCHAR(60) NOT NULL,
  capacity     INT NOT NULL,
  PRIMARY KEY (vehicleID),
  CHECK (capacity > 0)
) ENGINE=InnoDB;

-- 6. Seat (FK -> Vehicle)
CREATE TABLE IF NOT EXISTS Seat (
  seatID      INT NOT NULL AUTO_INCREMENT,
  vehicleID   INT NOT NULL,
  seat_num    VARCHAR(10) NOT NULL,
  seat_class  ENUM('Economy','Premium','Business','First','Standard') NOT NULL,
  PRIMARY KEY (seatID),
  UNIQUE (vehicleID, seat_num),
  FOREIGN KEY (vehicleID) REFERENCES Vehicle(vehicleID)
    ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB;

-- 7. Trip (FK -> Route, Vehicle)
CREATE TABLE IF NOT EXISTS Trip (
  tripID       INT NOT NULL,
  routeID      INT NOT NULL,
  vehicleID    INT NOT NULL,
  depart_time  DATETIME NOT NULL,
  arrival_time DATETIME NOT NULL,
  status       ENUM('Scheduled','Delayed','Cancelled','Departed','Arrived') NOT NULL DEFAULT 'Scheduled',
  PRIMARY KEY (tripID),
  FOREIGN KEY (routeID)  REFERENCES Route(routeID)
    ON DELETE RESTRICT ON UPDATE CASCADE,
  FOREIGN KEY (vehicleID) REFERENCES Vehicle(vehicleID)
    ON DELETE RESTRICT ON UPDATE CASCADE,
  CHECK (arrival_time > depart_time)
) ENGINE=InnoDB;

-- 8. Reservation (FK -> UserAccount, Trip)
CREATE TABLE IF NOT EXISTS Reservation (
  reservationID INT NOT NULL,
  accountID     INT NOT NULL,
  tripID        INT NOT NULL,
  status        ENUM('Pending','Paid','Ticketed','Cancelled') NOT NULL DEFAULT 'Pending',
  totalAmount   DECIMAL(10,2) NOT NULL DEFAULT 0.00,
  created_at    DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (reservationID),
  FOREIGN KEY (accountID) REFERENCES UserAccount(accountID)
    ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (tripID)    REFERENCES Trip(tripID)
    ON DELETE RESTRICT ON UPDATE CASCADE,
  CHECK (totalAmount >= 0)
) ENGINE=InnoDB;

-- 9. Passenger
CREATE TABLE IF NOT EXISTS Passenger (
  passengerID INT NOT NULL,
  name        VARCHAR(100) NOT NULL,
  DOB         DATE NULL,
  gender      ENUM('M','F','X','Other') NULL,
  PRIMARY KEY (passengerID)
) ENGINE=InnoDB;

-- 10. Ticket (FK -> Reservation, Passenger, Trip, Seat)
CREATE TABLE IF NOT EXISTS Ticket (
  ticketID      INT NOT NULL,
  reservationID INT NOT NULL,
  passengerID   INT NOT NULL,
  tripID        INT NOT NULL,
  seatID        INT NULL,
  status        ENUM('Open','Used','Refunded','Void') NOT NULL DEFAULT 'Open',
  issued_at     DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (ticketID),
  FOREIGN KEY (reservationID) REFERENCES Reservation(reservationID)
    ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (passengerID)   REFERENCES Passenger(passengerID)
    ON DELETE RESTRICT ON UPDATE CASCADE,
  FOREIGN KEY (tripID)        REFERENCES Trip(tripID)
    ON DELETE RESTRICT ON UPDATE CASCADE,
  FOREIGN KEY (seatID)        REFERENCES Seat(seatID)
    ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB;

-- 11. Payment (FK -> Reservation)
CREATE TABLE IF NOT EXISTS Payment (
  paymentID      INT NOT NULL,
  reservationID  INT NOT NULL,
  amount         DECIMAL(10,2) NOT NULL,
  status         ENUM('Authorized','Captured','Refunded','Failed','Voided') NOT NULL DEFAULT 'Authorized',
  payment_method ENUM('Card','Interac','Wallet','Cash','Other') NOT NULL,
  created_at     DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (paymentID),
  FOREIGN KEY (reservationID) REFERENCES Reservation(reservationID)
    ON DELETE CASCADE ON UPDATE CASCADE,
  CHECK (amount >= 0)
) ENGINE=InnoDB;

-- 12. Refund (weak entity; FK -> Payment)
CREATE TABLE IF NOT EXISTS Refund (
  paymentID  INT NOT NULL,
  refundNo   INT NOT NULL,
  amount     DECIMAL(10,2) NOT NULL,
  status     ENUM('Pending','Approved','Reversed') NOT NULL DEFAULT 'Pending',
  reason     VARCHAR(255) NULL,
  created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (paymentID, refundNo),
  FOREIGN KEY (paymentID) REFERENCES Payment(paymentID)
    ON DELETE CASCADE ON UPDATE CASCADE,
  CHECK (amount >= 0)
) ENGINE=InnoDB;

-- verify table list & count now:
SELECT table_name
FROM information_schema.tables
WHERE table_schema = 'eticket_reservation'
ORDER BY table_name;

SELECT COUNT(*) AS table_count
FROM information_schema.tables
WHERE table_schema = 'eticket_reservation';



-- INSERTS and Populating Tables

-- UserAccount
USE eticket_reservation;

INSERT INTO UserAccount (accountID, email, password) VALUES
(1, 'alice@example.com',  'hash_pw_1'),
(2, 'bob@example.com',    'hash_pw_2'),
(3, 'carol@example.com',  'hash_pw_3'),
(4, 'dave@example.com',   'hash_pw_4'),
(5, 'erin@example.com',   'hash_pw_5');

INSERT INTO Customer (accountID, name, DOB, SIN) VALUES
(1, 'Alice Johnson', '1992-05-14', '123456789'),
(2, 'Bob Smith',     '1988-11-02', '234567890'),
(3, 'Carol Li',      '1995-03-30', '345678901'),
(4, 'Dave Patel',    '1990-07-21', '456789012'),
(5, 'Erin Brown',    '1998-12-09', '567890123');

INSERT INTO CustomerPhone (accountID, phone_number, label) VALUES
(1, '+1-416-555-1001', 'Mobile'),
(1, '+1-416-555-2001', 'Home'),
(2, '+1-647-555-1002', 'Mobile'),
(3, '+1-905-555-1003', 'Mobile'),
(3, '+1-905-555-2003', 'Work'),
(4, '+1-289-555-1004', 'Mobile'),
(5, '+1-613-555-1005', 'Mobile');

-- Route
INSERT INTO Route (routeID, code, name, distance_km, duration_min) VALUES
(10, 'YYZ-YUL', 'Toronto → Montréal', 541.00, 75),
(11, 'YYZ-YVR', 'Toronto → Vancouver', 3359.00, 300),
(12, 'YUL-YOW', 'Montréal → Ottawa', 198.00, 120);

-- Vehicle
INSERT INTO Vehicle (vehicleID, vehicle_type, model, capacity) VALUES
(100, 'Aircraft', 'Airbus A220-300', 130),
(101, 'Train',    'Siemens Venture',  250),
(102, 'Bus',      'MCI J4500',         56);

-- Seats (a few per vehicle for demo; in reality you’d generate many)
INSERT INTO Seat (vehicleID, seat_num, seat_class) VALUES
(100, '1A',  'Business'),
(100, '1C',  'Business'),
(100, '12A', 'Economy'),
(100, '12B', 'Economy'),
(100, '12C', 'Economy'),

(101, '01', 'Standard'),
(101, '02', 'Standard'),
(101, '03', 'Standard'),
(101, '10', 'Premium'),

(102, 'A1', 'Standard'),
(102, 'A2', 'Standard'),
(102, 'B1', 'Standard'),
(102, 'B2', 'Standard');

-- Trip (ensure arrival_time > depart_time)
INSERT INTO Trip (tripID, routeID, vehicleID, depart_time, arrival_time, status) VALUES
(1000, 10, 100, '2025-10-03 08:00:00', '2025-10-03 09:15:00', 'Scheduled'), -- 75 min
(1001, 11, 100, '2025-10-05 09:00:00', '2025-10-05 14:00:00', 'Scheduled'), -- 300 min
(1002, 12, 101, '2025-10-07 13:00:00', '2025-10-07 15:00:00', 'Scheduled'), -- 120 min
(1003, 12, 102, '2025-10-10 10:30:00', '2025-10-10 12:30:00', 'Scheduled'); -- 120 min

-- Reservation
INSERT INTO Reservation (reservationID, accountID, tripID, status, totalAmount, created_at) VALUES
(5000, 1, 1000, 'Paid',     320.00, '2025-10-01 10:00:00'),
(5001, 2, 1000, 'Ticketed', 160.00, '2025-10-01 11:00:00'),
(5002, 3, 1001, 'Paid',     920.00, '2025-10-02 09:00:00'),
(5003, 4, 1002, 'Pending',  120.00, '2025-10-02 15:00:00'),
(5004, 5, 1002, 'Paid',     240.00, '2025-10-02 16:30:00'),
(5005, 1, 1003, 'Cancelled',100.00, '2025-10-02 18:00:00');

-- Passenger
INSERT INTO Passenger (passengerID, name, DOB, gender) VALUES
(900, 'Alice Johnson',   '1992-05-14', 'F'),
(901, 'Bob Smith',       '1988-11-02', 'M'),
(902, 'Carol Li',        '1995-03-30', 'F'),
(903, 'Daniel Chen',     '2001-04-12', 'M'),
(904, 'Emily Davis',     '1999-09-01', 'F'),
(905, 'Franklin Moore',  '1985-02-27', 'M');

-- Ticket (link res + pax + trip; some with seats)
-- trip 1000: seats 12A,12B used; 1C business used
-- Find actual seatIDs:
-- SELECT seatID, vehicleID, seat_num FROM Seat;
INSERT INTO Ticket (ticketID, reservationID, passengerID, tripID, seatID, status, issued_at)
SELECT 7000, 5000, 900, 1000, s.seatID, 'Open', '2025-10-01 10:05:00'
FROM Seat s WHERE s.vehicleID = 100 AND s.seat_num='12A';
INSERT INTO Ticket (ticketID, reservationID, passengerID, tripID, seatID, status, issued_at)
SELECT 7001, 5001, 901, 1000, s.seatID, 'Open', '2025-10-01 11:05:00'
FROM Seat s WHERE s.vehicleID = 100 AND s.seat_num='12B';
-- trip 1001: business seat 1C
INSERT INTO Ticket (ticketID, reservationID, passengerID, tripID, seatID, status, issued_at)
SELECT 7002, 5002, 902, 1001, s.seatID, 'Open', '2025-10-02 09:05:00'
FROM Seat s WHERE s.vehicleID = 100 AND s.seat_num='1C';
-- trip 1002: two passengers, one with seat, one without seat assignment yet
INSERT INTO Ticket (ticketID, reservationID, passengerID, tripID, seatID, status, issued_at)
SELECT 7003, 5003, 903, 1002, s.seatID, 'Open', '2025-10-02 15:05:00'
FROM Seat s WHERE s.vehicleID = 101 AND s.seat_num='02';
INSERT INTO Ticket (ticketID, reservationID, passengerID, tripID, seatID, status, issued_at)
VALUES (7004, 5004, 904, 1002, NULL, 'Open', '2025-10-02 16:35:00');
-- trip 1003: one ticket (cancelled reservation earlier; ticket may be void/refunded later)
INSERT INTO Ticket (ticketID, reservationID, passengerID, tripID, seatID, status, issued_at)
SELECT 7005, 5005, 900, 1003, s.seatID, 'Void', '2025-10-02 18:05:00'
FROM Seat s WHERE s.vehicleID = 102 AND s.seat_num='A1';

-- Payments
INSERT INTO Payment (paymentID, reservationID, amount, status, payment_method, created_at) VALUES
(8000, 5000, 320.00, 'Captured', 'Card',   '2025-10-01 10:10:00'),
(8001, 5001, 160.00, 'Captured', 'Interac','2025-10-01 11:10:00'),
(8002, 5002, 500.00, 'Captured', 'Card',   '2025-10-02 09:10:00'),
(8003, 5002, 420.00, 'Authorized','Card',  '2025-10-02 09:12:00'), -- part auth
(8004, 5003, 120.00, 'Failed',   'Card',   '2025-10-02 15:10:00'),
(8005, 5004, 240.00, 'Captured', 'Wallet', '2025-10-02 16:40:00'),
(8006, 5005, 100.00, 'Captured', 'Cash',   '2025-10-02 18:10:00');

-- Refunds (weak entity under Payment)
INSERT INTO Refund (paymentID, refundNo, amount, status, reason, created_at) VALUES
(8006, 1, 100.00, 'Approved', 'Reservation cancelled', '2025-10-03 09:00:00'),
(8001, 1,  20.00, 'Pending',  'Seat downgrade',        '2025-10-03 10:00:00');

-- === QUICK CHECKS (run and screenshot) ======================================
SELECT * FROM UserAccount ORDER BY accountID;
SELECT * FROM Customer ORDER BY accountID;
SELECT * FROM CustomerPhone ORDER BY accountID, phone_number;
SELECT * FROM Route ORDER BY routeID;
SELECT * FROM Vehicle ORDER BY vehicleID;
SELECT seatID, vehicleID, seat_num, seat_class FROM Seat ORDER BY vehicleID, seat_num;
SELECT * FROM Trip ORDER BY tripID;
SELECT * FROM Reservation ORDER BY reservationID;
SELECT * FROM Passenger ORDER BY passengerID;
SELECT * FROM Ticket ORDER BY ticketID;
SELECT * FROM Payment ORDER BY paymentID;
SELECT * FROM Refund ORDER BY paymentID, refundNo;

USE eticket_reservation;

-- View 1: Trip manifest (who’s on which trip, with seat & class)
CREATE OR REPLACE VIEW v_trip_manifest AS
SELECT 
  tk.tripID,
  r.code        AS route_code,
  r.name        AS route_name,
  tr.depart_time,
  tr.arrival_time,
  p.passengerID,
  p.name        AS passenger_name,
  s.seat_num,
  s.seat_class,
  tk.status     AS ticket_status
FROM Ticket tk
JOIN Trip   tr  ON tr.tripID = tk.tripID
JOIN Route  r   ON r.routeID = tr.routeID
JOIN Passenger p ON p.passengerID = tk.passengerID
LEFT JOIN Seat s ON s.seatID = tk.seatID
ORDER BY tr.depart_time, passenger_name;

-- View 2: Reservation financials (calculated captured + balance)
CREATE OR REPLACE VIEW v_reservation_financials AS
SELECT
  res.reservationID,
  res.accountID,
  res.tripID,
  res.status AS reservation_status,
  res.totalAmount,
  COALESCE(SUM(CASE WHEN pay.status='Captured' THEN pay.amount END),0) AS captured_amount,
  (res.totalAmount - COALESCE(SUM(CASE WHEN pay.status='Captured' THEN pay.amount END),0)) AS balance_due
FROM Reservation res
LEFT JOIN Payment pay ON pay.reservationID = res.reservationID
GROUP BY res.reservationID, res.accountID, res.tripID, res.status, res.totalAmount
ORDER BY res.reservationID;

-- View 3: Customer activity summary
CREATE OR REPLACE VIEW v_customer_activity AS
SELECT
  c.accountID,
  c.name AS customer_name,
  COUNT(DISTINCT res.reservationID) AS reservations_count,
  COUNT(DISTINCT tk.ticketID)       AS tickets_count,
  MAX(res.created_at)               AS last_reservation_at
FROM Customer c
LEFT JOIN Reservation res ON res.accountID = c.accountID
LEFT JOIN Ticket tk       ON tk.reservationID = res.reservationID
GROUP BY c.accountID, c.name
ORDER BY last_reservation_at DESC;

-- quick checks for screenshots
SELECT * FROM v_trip_manifest;
SELECT * FROM v_reservation_financials;
SELECT * FROM v_customer_activity;


-- Q1) Subquery (scalar): trips longer than their route’s average duration
SELECT 
  tr.tripID, r.code, r.name,
  TIMESTAMPDIFF(MINUTE, tr.depart_time, tr.arrival_time) AS scheduled_min,
  r.duration_min AS route_avg_min
FROM Trip tr
JOIN Route r ON r.routeID = tr.routeID
WHERE TIMESTAMPDIFF(MINUTE, tr.depart_time, tr.arrival_time) >
      (SELECT duration_min FROM Route WHERE routeID = tr.routeID)
ORDER BY tr.tripID;

-- Q2) Subquery (IN): customers with >1 phone number
SELECT c.accountID, c.name
FROM Customer c
WHERE c.accountID IN (
  SELECT accountID 
  FROM CustomerPhone 
  GROUP BY accountID 
  HAVING COUNT(*) > 1
)
ORDER BY c.accountID;

-- Q3) Correlated subquery: ticket count per reservation
SELECT 
  r.reservationID,
  r.accountID,
  (SELECT COUNT(*) FROM Ticket t WHERE t.reservationID = r.reservationID) AS ticket_count,
  r.totalAmount,
  r.status
FROM Reservation r
ORDER BY r.reservationID;

-- Q4) Window: rank routes by total captured revenue (desc)
WITH revenue_by_route AS (
  SELECT r.routeID, r.code, r.name,
         COALESCE(SUM(CASE WHEN p.status='Captured' THEN p.amount END),0) AS captured_revenue
  FROM Route r
  JOIN Trip tr        ON tr.routeID = r.routeID
  JOIN Reservation rs ON rs.tripID = tr.tripID
  LEFT JOIN Payment p ON p.reservationID = rs.reservationID
  GROUP BY r.routeID, r.code, r.name
)
SELECT *,
       RANK() OVER (ORDER BY captured_revenue DESC) AS revenue_rank
FROM revenue_by_route
ORDER BY revenue_rank, code;

-- Q5) Window: ROW_NUMBER per route by departure time
SELECT
  tr.tripID, r.code, tr.depart_time,
  ROW_NUMBER() OVER (PARTITION BY r.routeID ORDER BY tr.depart_time) AS trip_seq_on_route
FROM Trip tr
JOIN Route r ON r.routeID = tr.routeID
ORDER BY r.code, trip_seq_on_route;

-- Q6) Window: NTILE(4) on payment amount (quartiles)
SELECT
  paymentID, reservationID, amount, status,
  NTILE(4) OVER (ORDER BY amount) AS amount_quartile
FROM Payment
ORDER BY amount;

-- Q7) Window: running total of captured per reservation
SELECT
  reservationID, paymentID, created_at, status, amount,
  SUM(CASE WHEN status='Captured' THEN amount ELSE 0 END)
    OVER (PARTITION BY reservationID ORDER BY created_at ROWS UNBOUNDED PRECEDING)
    AS running_captured_total
FROM Payment
ORDER BY reservationID, created_at;

-- Q8) DISTINCT + ORDER BY: customers with paid/ticketed reservations
SELECT DISTINCT c.accountID, c.name
FROM Customer c
JOIN Reservation r ON r.accountID = c.accountID
WHERE r.status IN ('Paid','Ticketed')
ORDER BY c.name;

-- Q9) GROUP BY: trip revenue (captured only) + tickets issued
SELECT
  tr.tripID, r.code, r.name,
  COALESCE(SUM(CASE WHEN p.status='Captured' THEN p.amount END),0) AS captured_revenue,
  COUNT(DISTINCT tk.ticketID) AS tickets_issued
FROM Trip tr
JOIN Route r ON r.routeID = tr.routeID
LEFT JOIN Reservation rs ON rs.tripID = tr.tripID
LEFT JOIN Ticket tk ON tk.reservationID = rs.reservationID
LEFT JOIN Payment p ON p.reservationID = rs.reservationID
GROUP BY tr.tripID, r.code, r.name
ORDER BY captured_revenue DESC, tr.tripID;

-- Q10) Seat availability for trip 1000
SELECT 
  s.vehicleID, s.seat_num, s.seat_class
FROM Trip tr
JOIN Seat s ON s.vehicleID = tr.vehicleID
WHERE tr.tripID = 1000
  AND s.seatID NOT IN (
    SELECT t.seatID
    FROM Ticket t
    WHERE t.tripID = 1000
      AND t.seatID IS NOT NULL
  )
ORDER BY s.seat_num;

-- Q11) Outstanding balances (from view)
SELECT
  v.reservationID, v.accountID, v.tripID, v.reservation_status,
  v.totalAmount, v.captured_amount, v.balance_due
FROM v_reservation_financials v
WHERE v.balance_due > 0
ORDER BY v.balance_due DESC;

-- Q12) Top spenders by total captured (window)
WITH captured_by_account AS (
  SELECT
    r.accountID,
    COALESCE(SUM(CASE WHEN p.status='Captured' THEN p.amount END),0) AS total_captured
  FROM Reservation r
  LEFT JOIN Payment p ON p.reservationID = r.reservationID
  GROUP BY r.accountID
)
SELECT
  c.accountID,
  cu.name AS customer_name,
  c.total_captured,
  DENSE_RANK() OVER (ORDER BY c.total_captured DESC) AS spender_rank
FROM captured_by_account c
JOIN Customer cu ON cu.accountID = c.accountID
ORDER BY spender_rank, accountID;




