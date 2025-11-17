"""
E-Ticket Reservation System - Assignment 6 Demo App
Flask web UI that connects to the MySQL database and exposes
the advanced queries and views from Assignments 3–4.
"""

from flask import Flask, render_template
import mysql.connector as mysql

app = Flask(__name__)

# --- Database configuration (edit credentials if needed) ---
DB_CFG = dict(
    host="127.0.0.1",
    user="youruser",          # <-- change if your MySQL user is different
    password="yourpassword",      # <-- change to your MySQL password
    database="eticket_reservation"
)

def run_query(sql: str):
    """Utility to run a SELECT query and return (columns, rows)."""
    conn = mysql.connect(**DB_CFG)
    cur = conn.cursor()
    cur.execute(sql)
    cols = [d[0] for d in cur.description] if cur.description else []
    rows = cur.fetchall()
    cur.close()
    conn.close()
    return cols, rows

# ----------------- UI ROUTES -----------------

@app.route("/")
def home():
    """
    Home dashboard.
    Shows simple table counts to demonstrate that realistic dummy data
    is loaded and reachable.
    """
    counts_sql = """
    SELECT 'Customer' AS table_name, COUNT(*) AS row_count FROM Customer UNION ALL
    SELECT 'Reservation', COUNT(*) FROM Reservation UNION ALL
    SELECT 'Trip', COUNT(*) FROM Trip UNION ALL
    SELECT 'Ticket', COUNT(*) FROM Ticket UNION ALL
    SELECT 'Payment', COUNT(*) FROM Payment;
    """
    cols, rows = run_query(counts_sql)
    return render_template("home.html", cols=cols, rows=rows)


@app.route("/reports/top-spenders")
def top_spenders():
    """
    Advanced report (Q1): Top 3 customers by total captured payment.
    Uses GROUP BY and SUM over joins.
    """
    sql = """
    SELECT c.accountID,
           c.name,
           SUM(p.amount) AS total_spent
    FROM Customer c
    JOIN Reservation rs ON rs.accountID = c.accountID
    JOIN Payment     p  ON p.reservationID = rs.reservationID
    GROUP BY c.accountID, c.name
    ORDER BY total_spent DESC
    LIMIT 3;
    """
    cols, rows = run_query(sql)
    return render_template(
        "table.html",
        title="Top 3 Spenders",
        description="Advanced report using joins, aggregation and ORDER BY.",
        cols=cols,
        rows=rows,
        code=sql
    )


@app.route("/reports/routes-above-avg-distance")
def routes_above_avg():
    """
    Advanced report (Q2): Routes whose distance_km is above the global average.
    Demonstrates a scalar subquery in the WHERE clause.
    """
    sql = """
    SELECT routeID, code, name, distance_km
    FROM Route
    WHERE distance_km > (SELECT AVG(distance_km) FROM Route)
    ORDER BY distance_km DESC;
    """
    cols, rows = run_query(sql)
    return render_template(
        "table.html",
        title="Routes Longer Than Average Distance",
        description="Uses a subquery in WHERE to compare against AVG(distance_km).",
        cols=cols,
        rows=rows,
        code=sql
    )


@app.route("/reports/rank-routes-revenue")
def rank_routes():
    """
    Advanced report (Q4): Rank routes by captured revenue using a window function.
    This matches the query we used for the execution plan / optimization in A4.
    """
    sql = """
    WITH route_rev AS (
      SELECT r.routeID,
             r.code,
             r.name,
             COALESCE(SUM(p.amount),0) AS captured_revenue
      FROM Route r
      JOIN Trip         t  ON t.routeID  = r.routeID
      JOIN Reservation  rs ON rs.tripID  = t.tripID
      LEFT JOIN Payment p  ON p.reservationID = rs.reservationID
      GROUP BY r.routeID, r.code, r.name
    )
    SELECT *,
           RANK() OVER (ORDER BY captured_revenue DESC) AS revenue_rank
    FROM route_rev;
    """
    cols, rows = run_query(sql)
    return render_template(
        "table.html",
        title="Rank Routes by Captured Revenue",
        description="Uses CTE, aggregation and the RANK() window function.",
        cols=cols,
        rows=rows,
        code=sql
    )


@app.route("/views/trip-avg")
def view_trip_avg():
    """
    View 1: v_trip_avg_payment
    View with subquery in SELECT to compute average payment per trip.
    """
    sql = "SELECT * FROM v_trip_avg_payment;"
    cols, rows = run_query(sql)
    return render_template(
        "table.html",
        title="View: v_trip_avg_payment",
        description="View with a subquery in SELECT computing avg payment per trip.",
        cols=cols,
        rows=rows,
        code=sql
    )


@app.route("/views/customer-totals")
def view_customer_totals():
    """
    View 2: v_customer_totals
    Derived-table subquery in FROM to pre-aggregate reservations, tickets, payments.
    """
    sql = "SELECT * FROM v_customer_totals;"
    cols, rows = run_query(sql)
    return render_template(
        "table.html",
        title="View: v_customer_totals",
        description="View using a subquery in FROM to compute totals per customer.",
        cols=cols,
        rows=rows,
        code=sql
    )


@app.route("/views/active-customers")
def view_active_customers():
    """
    View 3: v_active_customers
    Subquery in WHERE to filter customers with positive total payments.
    """
    sql = "SELECT * FROM v_active_customers;"
    cols, rows = run_query(sql)
    return render_template(
        "table.html",
        title="View: v_active_customers",
        description="View using a subquery in WHERE to keep customers with payments > 0.",
        cols=cols,
        rows=rows,
        code=sql
    )


@app.route("/plan")
def show_plan():
    """
    Show the EXPLAIN ANALYZE plan for the route revenue ranking query.
    This corresponds to the execution plan analysed and optimised in A4.
    NOTE: Some MySQL installations may not allow EXPLAIN ANALYZE via client.
    """
    plan_sql = """
EXPLAIN ANALYZE
WITH route_rev AS (
  SELECT r.routeID, r.code, r.name,
         COALESCE(SUM(p.amount),0) AS captured_revenue
  FROM Route r
  JOIN Trip         t  ON t.routeID  = r.routeID
  JOIN Reservation  rs ON rs.tripID  = t.tripID
  LEFT JOIN Payment p  ON p.reservationID = rs.reservationID
  GROUP BY r.routeID, r.code, r.name
)
SELECT *,
       RANK() OVER (ORDER BY captured_revenue DESC) AS revenue_rank
FROM route_rev;
"""
    cols, rows = run_query(plan_sql)
    # EXPLAIN ANALYZE usually returns a single long text column.
    return render_template("plan.html", plan_rows=rows, code=plan_sql)


if __name__ == "__main__":
    # Debug mode is fine for assignment demo.
    app.run(debug=True)
