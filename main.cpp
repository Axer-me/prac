#include <iostream>
#include <cmath>

//Функция определения моментов инерции сечений на участках вала
double calculate_j(int form, float size, double formSech, double beta )
{
    const double PI = 3.14;
    if (form != 1 && form != 2 && form != 3)
    {
        std::cout << "fail";
        return(0);
    }
    else if (form == 1)
    {
        return(PI * pow(size, 4) / 32);
    }
    else if (form == 2)
    {
        return(beta * pow(size, 4));
    }
    else
    {
        return(PI * pow(size, 4) * (1 - pow(formSech, 4)) / 32);
    }
}

// Функция определения моментов сопротивления сечений участков вала
double calculate_w(int form, float size, double formSech, double alpha)
{
    const double PI = 3.14;
    if (form != 1 && form != 2 && form != 3)
    {
        std::cout << "fail";
        return(0);
    }
    else if (form == 1)
    {
        return(PI * pow(size, 3) / 16);
    }
    else if (form == 2)
    {
        return(alpha * pow(size, 3));
    }
    else
    {
        return(PI * pow(size, 3) * (1 - pow(formSech, 4)) / 16);
    }
}
// Функция определения моментов на участках вала по методу сечений,
// при переменной mode == true - на начале, false - в конце участка.
float calculate_M(float curM, float moment, float m, float length, bool mode)
{

    if (mode == true)
    {
        return(curM + moment);
    }
    else
    {
        return(curM + moment + (length * m));
    }
}

//Функция определения угла закручивания на участках
double calculate_angle(float moment, float length, int G, double j)
{
    return(((moment * length) * pow(10, 8) * pow(10, -6)) / (G * j));
}

int main()
{
   // переменные заданных участков
   //Первый участок
   float length1{};
   float size1{};
   float moment1{};
   float m1{};
   int form1{};
   double formSech1{};
   std::cout<< "Enter parameters of the first section: Length, Size, Moment, m, form(1 or 3), sectional form\n";
   std::cin>>length1>>size1>>moment1>>m1>>form1>>formSech1;
   //Второй участок
   float length2{};
   float size2{};
   float moment2{};
   float m2{};
   int form2{};
   double formSech2{};
   std::cout<< "\nEnter parameters of the second section: Length, Size, Moment, m, form(1,2 or 3), sectional form\n";
   std::cin>>length2>>size2>>moment2>>m2>>form2>>formSech2;
   //Третий участок
   float length3{};
   float size3{};
   float moment3{};
   float m3{};
   int form3{};
   double formSech3{};
   std::cout<< "\nEnter parameters of the third section: Length, Size, Moment, m, form(1, 2 or 3), sectional form\n";
   std::cin>>length3>>size3>>moment3>>m3>>form3>>formSech3;
   //Момент к конце вала и модуль сдвига G
   float momentEnd{};
   int G{};
   std::cout<<"\nEnter Moment at the end and G\n";
   std::cin>>momentEnd>>G;


   //Переменная condition отвечает за два чекбокса заделки в приложении-эталоне, в зависимости от значения рассчёты немного изменяются,
   // (condition==1) - заделка в начале
   // (condition==2) - заделка в конце
   // (condition==3) - заделка и в начале и в конце
   int condition{};
   std::cout<<"\nEnter condition:\n1 if sealed at the begining\n2 if sealed at the end\n3 if sealed at both ends\n";
   std::cin>>condition;

   //Табличные данные углов при прямоугольной форме
   double formSech[3] = { formSech1, formSech2, formSech3 };
   double alpha[3] = { 0, 0, 0 };
   double beta[3] = { 0, 0, 0 };
   //Приложение-эталон не даёт сделать первый участок вала прямоугольным, ограничиваем эту возможность
   if (form1 != 2)
   {
       if (form1 == 2 || form2 == 2 || form3 == 2)
       {
           double angles[13][3]{ {1, 0.208, 0.140}, {1.2, 0.219, 0.166}, {1.4, 0.228, 0.187},
               {1.6, 0.234, 0.204}, {1.8, 0.240, 0.217}, {2, 0.246, 0.229},
               {2.5, 0.258, 0.249}, {3, 0.267, 0.263}, {4, 0.282, 0.281},
               {6, 0.299, 0.299}, {8, 0.307, 0.307}, {10, 0.313, 0.313}, {11, 0.333, 0.333} };
           for (int j = 0; j < 3; j++)
           {
               for (int i = 0; i < 13; i++)
               {
                   if (angles[i][0] == formSech[j])
                   {
                       alpha[j] = angles[i][1];
                       beta[j] = angles[i][2];
                   }
               }
           }
       };
       if (condition != 1 && condition != 2 && condition != 3 )
       {
           std::cout <<"fail";
       }
       // в первом кондишене деталь(вал) имеет заделку в начале, счёт частей справа налево
       else if (condition == 1)
       {



           double j1 = calculate_j(form3, size3, formSech3, beta[2]);
           double j2 = calculate_j(form2, size2, formSech2, beta[1]);
           double j3 = calculate_j(form1, size1, formSech1, beta[0]);

           std::cout<<"Moments of inertia of shaft sections: " << j1 << "\t" << j2 << "\t" << j3 <<"\n";

           double w1 = calculate_w(form3, size3, formSech3, alpha[2]);
           double w2 = calculate_w(form2, size2, formSech2, alpha[1]);
           double w3 = calculate_w(form1, size1, formSech1, alpha[0]);

           std::cout<<"Moments of resistance of shaft sections: " << w1 << "\t" << w2 << "\t" << w3 <<"\n";


           float MStart1 = calculate_M(momentEnd, 0, m1, length1, true);
           float MEnd1 = calculate_M(momentEnd, 0, m1, length1, false);
           float MAvg1 = (MStart1 + MEnd1) / 2;

           float MStart2 = calculate_M(MEnd1, moment2, m2, length2, true);
           float MEnd2 = calculate_M(MEnd1, moment2, m2, length2, false);
           float MAvg2 = (MStart2 + MEnd2) / 2;

           float MStart3 = calculate_M(MEnd2, moment3, m3, length3, true);
           float MEnd3 = calculate_M(MEnd2, moment3, m3, length3, false);
           float MAvg3 = (MStart3 + MEnd3) / 2;

           std::cout<<"Moments on shaft sections are determined using the section method: \n"
                      "First section: Start: " << MStart1 << "\t End: " << MEnd1 << " \t Average: " << MAvg1 << "\n"
                      "Second section: Start: " << MStart2 << "\t End: " << MEnd2 << " \t Average: " << MAvg2 << "\n"
                       "Third section: Start: " << MStart3 << "\t End: " << MEnd3 << " \t Average: " << MAvg3 << "\n";


           double angle1 = calculate_angle(MAvg1, length3, G, j1);
           double angle2 = calculate_angle(MAvg2, length2, G, j2);
           double angle3 = calculate_angle(MAvg3, length1, G, j3);

           std::cout<< "The angle of twist in one area is determined by the formula.\n "
                       "First section: " << angle1 << "\t Second section: " << angle2 << "\t Third section: " << angle3 << "\n";

       }
       // во втором кондишене деталь(вал) имеет заделку в конце, счёт частей слева направо
       else if (condition == 2)
       {
           //Направление моментов при изменении направления измерений обозначаем "-"
           moment1 = moment1 * (-1);
           moment2 = moment2 * (-1);
           moment3 = moment3 * (-1);
           m1 = m1 * (-1);
           m2 = m2 * (-1);
           m3 = m3 * (-1);
           //Дальше в рассчётах я поменял местами несколько переменных,
           // но в остальном, ровно то же самое, что и в первом кондишене
           double j3 = calculate_j(form3, size3, formSech3, beta[2]);
           double j2 = calculate_j(form2, size2, formSech2, beta[1]);
           double j1 = calculate_j(form1, size1, formSech1, beta[0]);

           std::cout<<"Moments of inertia of shaft sections: " << j1 << "\t" << j2 << "\t" << j3 <<"\n";

           double w3 = calculate_w(form3, size3, formSech3, alpha[2]);
           double w2 = calculate_w(form2, size2, formSech2, alpha[1]);
           double w1 = calculate_w(form1, size1, formSech1, alpha[0]);

           std::cout<<"Moments of resistance of shaft sections: " << w1 << "\t" << w2 << "\t" << w3 <<"\n";

           float MStart1 = calculate_M(0, moment1, m1, length1, true);
           float MEnd1 = calculate_M(0, moment1, m1, length1, false);
           float MAvg1 = (MStart1 + MEnd1) / 2;

           float MStart2 = calculate_M(MEnd1, moment2, m2, length2, true);
           float MEnd2 = calculate_M(MEnd1, moment2, m2, length2, false);
           float MAvg2 = (MStart2 + MEnd2) / 2;

           float MStart3 = calculate_M(MEnd2, moment3, m3, length3, true);
           float MEnd3 = calculate_M(MEnd2, moment3, m3, length3, false);
           float MAvg3 = (MStart3 + MEnd3) / 2;

           std::cout<<"Moments on shaft sections are determined using the section method: \n"
                      "First section: Start: " << MStart1 << "\t End: " << MEnd1 << " \t Average: " << MAvg1 << "\n"
                      "Second section: Start: " << MStart2 << "\t End: " << MEnd2 << " \t Average: " << MAvg2 << "\n"
                       "Third section: Start: " << MStart3 << "\t End: " << MEnd3 << " \t Average: " << MAvg3 << "\n";

           double angle1 = calculate_angle(MAvg1, length1, G, j1);
           double angle2 = calculate_angle(MAvg2, length2, G, j2);
           double angle3 = calculate_angle(MAvg3, length3, G, j3);

           std::cout<< "The angle of twist in one area is determined by the formula.\n "
                       "First section: " << angle1 << "\t Second section: " << angle2 << "\t Third section: " << angle3 << "\n";


       }
       // во втором кондишене деталь(вал) имеет заделку и в начале и в конце
       else if(condition == 3)
       {
           moment1 = moment1 * (-1);
           moment2 = moment2 * (-1);
           moment3 = moment3 * (-1);
           m1 = m1 * (-1);
           m2 = m2 * (-1);
           m3 = m3 * (-1);

           double j3 = calculate_j(form3, size3, formSech3, beta[2]);
           double j2 = calculate_j(form2, size2, formSech2, beta[1]);
           double j1 = calculate_j(form1, size1, formSech1, beta[0]);

           std::cout<<"Moments of inertia of shaft sections: " << j1 << "\t" << j2 << "\t" << j3 <<"\n";

           double w3 = calculate_w(form3, size3, formSech3, alpha[2]);
           double w2 = calculate_w(form2, size2, formSech2, alpha[1]);
           double w1 = calculate_w(form1, size1, formSech1, alpha[0]);

           std::cout<<"Moments of resistance of shaft sections: " << w1 << "\t" << w2 << "\t" << w3 <<"\n";

           //В этом кондишене используется другое решение, поэтому реализация будет чуть сложнее

           std::cout << "The circuit is statically indeterminate. The solution of such a scheme involves the disclosure of static indetermination. \n"
                        "Let's discard the conditionally left support and determine the moments and angles of twist on the shaft sections without taking into account the left support.\n";

           float MStart1 = calculate_M(0, 0, m1, length1, true);
           float MEnd1 = calculate_M(0, 0, m1, length1, false);
           float MAvg1 = (MStart1 + MEnd1) / 2;

           float MStart2 = calculate_M(MEnd1, moment2, m2, length2, true);
           float MEnd2 = calculate_M(MEnd1, moment2, m2, length2, false);
           float MAvg2 = (MStart2 + MEnd2) / 2;

           float MStart3 = calculate_M(MEnd2, moment3, m3, length3, true);
           float MEnd3 = calculate_M(MEnd2, moment3, m3, length3, false);
           float MAvg3 = (MStart3 + MEnd3) / 2;

           std::cout<<"Moments on shaft sections are determined using the section method: \n"
                      "First section: Start: " << MStart1 << "\t End: " << MEnd1 << " \t Average: " << MAvg1 << "\n"
                      "Second section: Start: " << MStart2 << "\t End: " << MEnd2 << " \t Average: " << MAvg2 << "\n"
                       "Third section: Start: " << MStart3 << "\t End: " << MEnd3 << " \t Average: " << MAvg3 << "\n"
                        "In sections with a distributed moment, we substitute the average value of the moment in the section into the formula\n";

           double angle1 = calculate_angle(MAvg1, length1, G, j1);
           double angle2 = calculate_angle(MAvg2, length2, G, j2);
           double angle3 = calculate_angle(MAvg3, length3, G, j3);

           std::cout<< "First section: " << angle1 << "\t Second section: " << angle2 << "\t Third section: " << angle3 << "\n";

           double angleSum = angle1 + angle2 + angle3;

           std::cout<< "The total angle of rotation of the shaft due to the applied load:" << angleSum << "\n";

           double angleR = calculate_angle(1, length1, G, j1) + calculate_angle(1, length2, G, j2) + calculate_angle(1, length3, G, j3);

           std::cout << "Angle of rotation of the shaft due to the action of the rejected reaction of the support R:\t" << angleR << "\n";

           double R = (angleSum / angleR) * (-1);

           std::cout<< "From here we find the reaction of the discarded left support R:" << R << "\n";

           MStart1 = calculate_M(0, R, m1, length1, true);
           MEnd1 = calculate_M(0, R, m1, length1, false);
           MAvg1 = (MStart1 + MEnd1) / 2;

           MStart2 = calculate_M(MEnd1, moment2, m2, length2, true);
           MEnd2 = calculate_M(MEnd1, moment2, m2, length2, false);
           MAvg2 = (MStart2 + MEnd2) / 2;

           MStart3 = calculate_M(MEnd2, moment3, m3, length3, true);
           MEnd3 = calculate_M(MEnd2, moment3, m3, length3, false);
           MAvg3 = (MStart3 + MEnd3) / 2;

           std::cout<<"Now, having the reaction of the left support, we calculate the shaft as statically determinate\n"
                      "Moments on shaft sections are determined using the section method: \n"
                      "First section: Start: " << MStart1 << "\t End: " << MEnd1 << " \t Average: " << MAvg1 << "\n"
                      "Second section: Start: " << MStart2 << "\t End: " << MEnd2 << " \t Average: " << MAvg2 << "\n"
                       "Third section: Start: " << MStart3 << "\t End: " << MEnd3 << " \t Average: " << MAvg3 << "\n";

           angle1 = calculate_angle(MAvg1, length1, G, j1);
           angle2 = calculate_angle(MAvg2, length2, G, j2);
           angle3 = calculate_angle(MAvg3, length3, G, j3);

           std::cout<< "The angle of twist in one area is determined by the formula.\n "
                       "First section: " << angle1 << "\t Second section: " << angle2 << "\t Third section: " << angle3 << "\n";

       }
   }
   else std::cout<<"fail";
}

