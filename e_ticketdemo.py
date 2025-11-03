import mysql.connector as mysql
from tabulate import tabulate

queries = {
"Q1":"SELECT c.accountID,c.name,SUM(p.amount) total_spent FROM Customer c JOIN Reservation rs ON rs.accountID=c.accountID JOIN Payment p ON p.reservationID=rs.reservationID GROUP BY c.accountID,c.name ORDER BY total_spent DESC LIMIT 3;",
"Q2":"SELECT routeID,code,name,distance_km FROM Route WHERE distance_km>(SELECT AVG(distance_km) FROM Route) ORDER BY distance_km DESC;",
"Q3":"SELECT t.tripID,'Future' time_bucket FROM Trip t WHERE t.depart_time>=NOW() UNION SELECT t.tripID,'Past' FROM Trip t WHERE t.depart_time<NOW() ORDER BY 2,1;",
"Q4":"WITH route_rev AS(SELECT r.routeID,r.code,r.name,COALESCE(SUM(p.amount),0) captured_revenue FROM Route r JOIN Trip t ON t.routeID=r.routeID JOIN Reservation rs ON rs.tripID=t.tripID LEFT JOIN Payment p ON p.reservationID=rs.reservationID GROUP BY r.routeID,r.code,r.name) SELECT *,RANK()OVER(ORDER BY captured_revenue DESC) revenue_rank FROM route_rev;",
"Q5":"SELECT reservationID,ticketID,passengerID,seatID,ROW_NUMBER()OVER(PARTITION BY reservationID ORDER BY ticketID) ticket_seq FROM Ticket ORDER BY reservationID,ticket_seq;",
"Q6":"SELECT tripID,ticketID,passengerID,NTILE(4)OVER(PARTITION BY tripID ORDER BY ticketID) quartile_in_trip FROM Ticket ORDER BY tripID,quartile_in_trip,ticketID;",
"Q7":"SELECT t.* FROM Ticket t WHERE t.issued_at=(SELECT MIN(t2.issued_at) FROM Ticket t2 WHERE t2.reservationID=t.reservationID);",
"Q8":"WITH RECURSIVE days(d)AS(SELECT DATE(NOW()) UNION ALL SELECT DATE_ADD(d,INTERVAL 1 DAY) FROM days WHERE d<DATE_ADD(DATE(NOW()),INTERVAL 6 DAY)) SELECT r.code route_code,d service_date FROM days,Route r WHERE r.code='YYZ-YUL' ORDER BY d;",
"Q9":"SELECT r.routeID,r.code,r.name,AVG(TIMESTAMPDIFF(MINUTE,t.depart_time,t.arrival_time)) avg_min,STDDEV_POP(TIMESTAMPDIFF(MINUTE,t.depart_time,t.arrival_time)) sd_min FROM Route r JOIN Trip t ON t.routeID=r.routeID GROUP BY r.routeID,r.code,r.name ORDER BY avg_min;"
}

def show(cur,q):
    cur.execute(q)
    rows=cur.fetchall()
    print(tabulate(rows,headers=[d[0] for d in cur.description],tablefmt='github'))

def main():
    conn=mysql.connect(host='localhost',user='root',password='cs3319',database='eticket_reservation')
    cur=conn.cursor()
    while True:
        for i,k in enumerate(queries,1): print(f"{i}. {k}")
        c=input("Select(1-9,q): ")
        if c.lower()=='q':break
        if c.isdigit() and 1<=int(c)<=9: show(cur,list(queries.values())[int(c)-1])
    cur.close();conn.close()

if __name__=="__main__":main()
