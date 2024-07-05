#include <iostream>
#include <cmath>
#include <beam.h>
#include <vector>
#include <ios>
#include <limits>

int main()
{
    //    qDebug("1");
    // переменные заданных участков
    //Первый участок
    BeamAnalyzer beam;


    double length1{};
    double size1{};
    double moment1{};
    double m1{};
    int form1{};
    double formSech1{};
    std::cout<< "Enter parameters of the first section: Length, Size, Moment, m, form(0,2), sectional form\n";
    std::cin>>length1>>size1>>moment1>>m1>>form1>>formSech1;
    if(form1 == 0)
    {
        beam.AddRoundSegment(length1, size1, moment1, m1);
    }
    else if (form1 == 2)
    {
        beam.AddPipeSegment(length1, size1, moment1, m1, formSech1);
    }
    else std::cout<< "\nError\n";
    //Второй участок
    double length2{};
    double size2{};
    double moment2{};
    double m2{};
    int form2{};
    double formSech2{};
    std::cout<< "\nEnter parameters of the second section: Length, Size, Moment, m, form(0,1,2), sectional form\n";
    std::cin>>length2>>size2>>moment2>>m2>>form2>>formSech2;
    if(form2 == 0)
    {
        beam.AddRoundSegment(length2, size2, moment2, m2);
    }
    else if (form2 == 1)
    {
        beam.AddRectangularSegment(length2, size2, moment2, m2, formSech2);
    }
    else if (form2 == 2)
    {
        beam.AddPipeSegment(length2, size2, moment2, m2, formSech2);
    }
    double length3{};
    double size3{};
    double moment3{};
    double m3{};
    int form3{};
    double formSech3{};
    std::cout<< "\nEnter parameters of the third section: Length, Size, Moment, m, form(0,1,2), sectional form\n";
    std::cin>>length3>>size3>>moment3>>m3>>form3>>formSech3;
    if(form3 == 0)
    {
        beam.AddRoundSegment(length3, size3, moment3, m3);
    }
    else if (form3 == 1)
    {
        beam.AddRectangularSegment(length3, size3, moment3, m3, formSech3);
    }
    else if (form3 == 2)
    {
        beam.AddPipeSegment(length3, size3, moment3, m3, formSech3);
    }
    std::cout<<length3 << "\t" << size3 << "\t" << moment3 << "\t" << m3 << "\t" << formSech3 << "\n";
    double momentEnd{};
    double G{};
    std::cout<<"\nEnter Moment at the end and G\n";
    std::cin>>momentEnd>>G;
    beam.SetMomentAtTheEnd(momentEnd);
    beam.SetShearModulus(G);
    std::cout<< "Shear modulus:" << beam.GetShearModulus();
    //Переменная condition отвечает за два чекбокса заделки в приложении-эталоне, в зависимости от значения рассчёты немного изменяются,
    // (condition==1) - заделка в начале
    // (condition==2) - заделка в конце
    // (condition==3) - заделка и в начале и в конце
    int condition{};
    std::cout<<"\nEnter condition:\n1 if sealed at the begining\n2 if sealed at the end\n3 if sealed at both ends\n";
    std::cin>>condition;
    beam.SetCondition(condition);
    beam.SegmentShapeCheck();
    std::cout << "Shapes checked!\n";
    std::cout<<"Condition: " << beam.GetCondition() << "\tSegment count: " << beam.GetSegmentCount() << "\tShear modulus: " << beam.GetShearModulus() << "\n";
    beam.Build(condition);
    std::cout<< "Beam built!\n";
    std::cout<< "Angle1: " << beam.GetSegmentTwistAngles(0);
    std::cout<< "\nAngle2: " << beam.GetSegmentTwistAngles(1);
    std::cout<< "\nAngle3: " << beam.GetSegmentTwistAngles(2);

}

