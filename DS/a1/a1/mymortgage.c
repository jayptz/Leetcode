#include <math.h>
#include "mymortgage.h"
/**
 * Compute the monthly payment of given mortgage princile, annual interest rate (%), and mortgage years. 
 *
 * @param principal_amount - float type.
 * @param annual_interest_rate - value of parcentage rate, e.g. 5 means 5%, float type.
 * @param years - number of mortgage year, int type.
 * @return - monthly payment, float type.
 */
float monthly_payment(float principal_amount, float annual_interest_rate, int years)
{
    if (years <= 0 || annual_interest_rate < 0) return 0.0f;
    
    float monthly_rate = annual_interest_rate / 100.0f / 12.0f;  
    int total_months = years * 12;
    
    if (monthly_rate == 0.0f) {
       
        return principal_amount / total_months;
    }
    
    float monthly_payment = principal_amount * (monthly_rate * pow(1 + monthly_rate, total_months)) / 
                           (pow(1 + monthly_rate, total_months) - 1);
    
    return monthly_payment;
}

/**
 * Determine the total payment of loan given mortgage princile, annual interest rate (%), and mortgage years. 
 *
 * @param principal_amount - float type.
 * @param annual_interest_rate - value of parcentage rate, e.g. 5 means 5%, float type.
 * @param years - number of mortgage year, int type.
 * @return - the total payment of the loan, float type.
 */
float total_payment(float principal_amount, float annual_interest_rate, int years)
{
    float monthly_pay = monthly_payment(principal_amount, annual_interest_rate, years);
    if (monthly_pay == 0.0f) return 0.0f;
    
    int total_months = years * 12;
    return monthly_pay * total_months;
}

/**
 * Determine the total interested payed for the loan given mortgage princile, annual interest rate (%), and mortgage years. 
 *
 * @param principal_amount - float type.
 * @param annual_interest_rate - value of parcentage rate, e.g. 5 means 5%, float type.
 * @param years - number of mortgage year, int type.
 * @return - the total interest payed by the end of paying off the loan, float type.
 */
float total_interest(float principal_amount, float annual_interest_rate, int years)
{
    float total_pay = total_payment(principal_amount, annual_interest_rate, years);
    if (total_pay == 0.0f) return 0.0f;
    
    return total_pay - principal_amount;
}
