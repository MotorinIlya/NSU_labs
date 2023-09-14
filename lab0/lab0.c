#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

char Get_value_of_symbol (int number)
{
    char point = ' ';
    if (number >= 0 && number <= 9)
    {
        point = (char) (number + '0');
    }
    else if (number >= 10 && number <= 15)
    {
        point = (char) (number + 'a' - 10);
    }
    return point;
}

int Get_value_of_digit (char point)
{
    int number = 0;
    if (point >= '0' && point <= '9')
    {
        number = point - '0';
    }
    else if (point >= 'a' && point <= 'f')
    {
        number = point - 'a' + 10;
    }
    else if (point >= 'A' && point <= 'F')
    {
        number = point - 'A' + 10;
    }
    return number;
}

int Calc_length (const char mass[])
{
    int count = 0;
    while (mass[count] != '\0')
    {
        count++;
    }
    return count;
}

_Bool Has_dot (const char mass[]) {
    for (int i = 0; i < Calc_length(mass); i++)
    {
        if (mass[i] == '.')
        {
            return true;
        }
    }
    return false;
}

void Reverse_string (char mass[])
{
    for (int i = 0, j = Calc_length(mass) - 1; i < j; i++, j--)
    {
        char c = mass[i];
        mass[i] = mass[j];
        mass[j] = c;
    }
}

float Calc_exponent (int basis_degree, int indicator_degree)
{
    float compositions = 1;
    if (indicator_degree > 0)
    {
        for (int i = 0; i < indicator_degree; i++)
        {
            compositions *= basis_degree;
        }
    }
    else if (indicator_degree < 0)
    {
        for (int i = 0; i > indicator_degree; i--)
        {
            compositions /= basis_degree;
        }
    }
    return compositions;
}

int Calc_number_after_point (const char digits[])
{
    int count = 0;
    _Bool find_dot = false;
    for (int i = 0; i < Calc_length(digits); i++)
    {
        if (digits[i] == '.')
        {
            find_dot = true;
        }
        if (find_dot == true)
        {
            count++;
        }
    }
    return count;
}

_Bool Verify_of_number_system (int first_number_system, int second_number_system)
{
    if (first_number_system >= 2 && first_number_system <= 16 && second_number_system >= 2 && second_number_system <= 16)
    {
        return true;
    }
    return false;
}

_Bool Verify_on_accuracy_number (int number_system, const char line[])
{
    int max = 0;
    for (int i = 0; i < Calc_length(line); i++)
    {
        int point = Get_value_of_digit(line[i]);
        if (point > max)
        {
            max = point;
        }
    }
    return (max < number_system);
}

_Bool Verify_on_accuracy_point_of_number (const char line[]) {
    for (int i = 0; i < Calc_length(line); i++)
    {
        if (!((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'a' && line[i] <= 'f') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] == '.')))
        {
            return false;
        }
    }
    return true;
}

_Bool Verify_on_accuracy_dot (const char line[])
{
    return !((line[Calc_length(line) - 1] == '.') || (line[0] == '.'));
}

int Count_dots (const char line[])
{
    int count = 0;
    for (int i = 0; i < Calc_length(line); i++)
    {
        if (line[i] == '.')
        {
            count++;
        }
    }
    return count;
}

_Bool Full_verify (int first_number_sys, int second_number_sys, char number[])
{
    _Bool check_num_sys = Verify_of_number_system(first_number_sys, second_number_sys);
    _Bool check_accuracy_num = Verify_on_accuracy_number(first_number_sys, number);
    _Bool check_accuracy_digit = Verify_on_accuracy_point_of_number(number);
    _Bool check_dot = Verify_on_accuracy_dot(number);
    if (!(check_num_sys && check_accuracy_num && check_accuracy_digit && check_dot))
    {
        return false;
    }
    if (Has_dot(number))
    {
        if (Count_dots(number) != 1)
        {
            return false;
        }
    }
    return true;
}

void Make_and_count_array_values_without_dot (const char number[], int *count_element_without_dot, int *digit_values)
{
    for (int i = 0; i < Calc_length(number); i++)
    {
        if (number[i] != '.')
        {
            int value = Get_value_of_digit(number[i]);
            digit_values[*count_element_without_dot] = value;
            *count_element_without_dot += 1;
        }
    }
}

int Get_exponent_offset (const char number_array[])
{
    if (!Has_dot(number_array))
    {
        return 1;
    }
    else
    {
        return Calc_number_after_point(number_array);
    }
}

double Count_value_number (const int count_element_without_dot, int *digit_values,const int number_system, const int exponent_offset)
{
    double value_number = 0;
    for (int i = 0; i < count_element_without_dot; ++i)
    {
        value_number += digit_values[i] * Calc_exponent(number_system, count_element_without_dot - exponent_offset - i);
    }
    return value_number;
}

double Convert_to_number (int number_system, char number[])
{
    int count_element_without_dot = 0;
    int digit_values[13] = {0};
    Make_and_count_array_values_without_dot(number, &count_element_without_dot, digit_values);
    int exponent_offset = Get_exponent_offset(number);
    return Count_value_number(count_element_without_dot, digit_values, number_system, exponent_offset);
}

char *Make_number (int count, int count_frac, const char integer_chars[], const char chars_after_dot[])
{
    int n = 0;
    char *final_number = malloc(count + count_frac + 1 + 1);
    if (count == 0)
    {
        final_number[0] = '0';
        n++;
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            final_number[n] = integer_chars[i];
            n++;
        }
    }
    if (count_frac != 0)
    {
        final_number[n] = '.';
        n++;
        for (int i = 0; i < count_frac; i++)
        {
            final_number[n] = chars_after_dot[i];
            n++;
        }
    }
    final_number[n] = '\0';
    return final_number;
}

void Get_and_count_integer_elements (long long full_number, long long number_system, int *integer_elements, int *count)
{
    *count = 0;
    for (int i = 0; full_number != 0; i++)
    {
        integer_elements[i] = full_number % number_system;
        full_number /= number_system;
        *count += 1;
    }
    if (*count == 0)
    {
        *count = 1;
        integer_elements[0] = 0;
    }
}

void Get_and_count_fractional_elements (double fractional, long long number_system, int *fractional_elements, int *count_frac)
{
    *count_frac = 0;
    for (int i = 0; (fractional != 0) && (i < 12); i++)
    {
        fractional *= number_system;
        double full_part;
        fractional = modf(fractional, &full_part);
        fractional_elements[i] = (int) full_part;
        *count_frac += 1;
    }
}

void Create_char_array_from_int (int count, int int_elements[], char *char_elements)
{
    for (int i = 0; i < count; i++)
    {
        char_elements[i] = Get_value_of_symbol(int_elements[i]);
    }
    char_elements[count] = '\0';
}

char *Create_to_n_number_system (int number_system, double number)
{
    double full_number;
    double fractional = modf(number, &full_number);
    int integer_elements[256];
    int count;
    Get_and_count_integer_elements(full_number, number_system, integer_elements, &count);
    char integer_chars[count + 1];
    Create_char_array_from_int(count, integer_elements, integer_chars);
    Reverse_string(integer_chars);
    int fractional_elements[12];
    int count_frac = 0;
    Get_and_count_fractional_elements(fractional, number_system, fractional_elements, &count_frac);
    char chars_after_dot[count_frac + 1];
    Create_char_array_from_int(count_frac, fractional_elements, chars_after_dot);
    char *final_number = Make_number(count,count_frac,integer_chars,chars_after_dot);
    return final_number;
}

void Delete_translation_symbol (char p[])
{
    int i = 0;
    while (p[i] != '\n')
    {
        i++;
    }
    p[i] = '\0';
}

int main ()
{
    int first_number_sys, second_number_sys;
    if (scanf("%d %d\n", &first_number_sys, &second_number_sys) != 2)
    {
        printf("bad input");
        return 0;
    }
    char number_to_array[15];
    if (fgets(number_to_array, 15, stdin) == NULL)
    {
        printf("bad input");
        return 0;
    }
    Delete_translation_symbol(number_to_array);
    if (!Full_verify(first_number_sys, second_number_sys, number_to_array))
    {
        printf("bad input");
        return 0;
    }
    double number = Convert_to_number(first_number_sys, number_to_array);
    char *p = Create_to_n_number_system(second_number_sys, number);
    printf("%s", p);
    free(p);
    return 0;
}
