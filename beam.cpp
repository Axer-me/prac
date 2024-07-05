#include <cmath>
#include <iostream>
//#include <QImage>
//#include <QPainter>
//#include <QRect>
//#include <QFont>
//#include <QPainterPath>
//#include <QTransform>

#include <beam.h>


BeamAnalyzer::BeamAnalyzer()
{
    m_segments = std::vector<std::shared_ptr<Segment>>();
}



// Нужно убрать неиспользованные члены класса, чтобы заработало
void BeamAnalyzer::AddRoundSegment( double length, double size, double moment, double m )
{
    std::shared_ptr<Segment> roundSegment = std::make_shared<RoundSegment>(length,  size,  moment,  m );
    AddSegment( roundSegment );
}

void BeamAnalyzer::AddRectangularSegment( double length, double size, double moment, double m, double formSech )
{
    std::shared_ptr<Segment> rectangularSegment = std::make_shared<RectangularSegment>(length,  size,  moment,  m,  formSech );
    AddSegment( rectangularSegment );
}

void BeamAnalyzer::AddPipeSegment( double length, double size, double moment, double m, double formSech )
{
    std::shared_ptr<Segment> pipeSegment = std::make_shared<PipeSegment>(length,  size,  moment,  m,  formSech );
    AddSegment( pipeSegment );
}

void BeamAnalyzer::SegmentShapeCheck()
{
    for (size_t i = 0; i < m_segments.size(); i++)
    {
        if (static_cast<int>(m_segments[i]->GetShape()) != 0 && static_cast<int>(m_segments[i]->GetShape()) != 1 && static_cast<int>(m_segments[i]->GetShape()) != 2)
        {
            std::cout<< "wrong shapes\n";

        }
        else std::cout<< "everything is fine\n";
    }
}

BeamAnalyzer::~BeamAnalyzer()
{
}

void BeamAnalyzer::DeleteSegment( size_t index )
{
    m_segments.erase( m_segments.begin() + index );
}

size_t BeamAnalyzer::GetSegmentCount() const
{
    return m_segments.size();
}

void BeamAnalyzer::SetCondition( int condition )
{
    m_condition = condition;
}

int BeamAnalyzer::GetCondition() const
{
    return m_condition;
}

void BeamAnalyzer::SetMomentAtTheEnd( double moment )
{
    m_momentAtTheEnd = moment;
}

double BeamAnalyzer::GetMomentAtTheEnd() const
{
    return m_momentAtTheEnd;
}

void BeamAnalyzer::SetShearModulus( double pressure )
{
    m_shearModulus = pressure;
}

double BeamAnalyzer::GetShearModulus() const
{
    return m_shearModulus;
}

void BeamAnalyzer::AddSegment( std::shared_ptr<Segment> segment )
{
    segment->momentOfInertia = segment->GetMomentOfInertia();
    segment->momentOfResistance = segment->GetMomentOfResistance();
    m_segments.push_back( segment );

}

double BeamAnalyzer::GetSegmentTwistAngles(size_t segmentIndex) const
{
    m_segments[segmentIndex]->twistAngle = (m_segments[segmentIndex]->segmentMomentStart + m_segments[segmentIndex]->segmentMomentEnd) / 2 * m_segments[segmentIndex]->length * pow(10, 2)
            / GetShearModulus() / m_segments[segmentIndex]->momentOfInertia ;
    return (m_segments[segmentIndex]->twistAngle);
}

void BeamAnalyzer::CalculateSegmentMoments(int condition)
{
    if (condition !=1 && condition !=2 && condition !=3)
    {
        //qWarning
    }
    else if (condition == 1)
    {


        double cur_moment = GetMomentAtTheEnd();
        for (size_t i = m_segments.size() ; i > 0; i--)
        {
            if (i == m_segments.size())
            {
                m_segments[m_segments.size() - i]->segmentMomentStart = cur_moment + 0;
                m_segments[m_segments.size() - i]->segmentMomentEnd =
                        m_segments[m_segments.size() - i]->segmentMomentStart + (m_segments[i-1]->length * m_segments[i-1]->m);
                cur_moment = m_segments[m_segments.size() - i]->segmentMomentEnd;
            }
            else
            {
                m_segments[m_segments.size() - i]->segmentMomentStart = cur_moment + m_segments[i]->moment;
                m_segments[m_segments.size() - i]->segmentMomentEnd =
                        m_segments[m_segments.size() - i]->segmentMomentStart + (m_segments[i-1]->length * m_segments[i-1]->m);
                cur_moment = m_segments[m_segments.size() - i]->segmentMomentEnd;
            }
        }

    }
    else if (condition == 2)
    {
        double cur_moment = 0;
        for (size_t i = 0 ; i < m_segments.size(); i++)
        {
            m_segments[i]->segmentMomentStart = cur_moment - m_segments[i]->moment;
            std::cout<<m_segments[i]->segmentMomentStart<< "\n";
            m_segments[i]->segmentMomentEnd = m_segments[i]->segmentMomentStart - (m_segments[i]->length * m_segments[i]->m);
            std::cout<<m_segments[i]->segmentMomentEnd<< "\n";
            cur_moment = m_segments[i]->segmentMomentEnd;
        }
    }
    else
    {
        double cur_moment = 0;
        double angleSum = 0;
        double angleR = 0;
        double ShearModulus = GetShearModulus();
        m_segments[0]->moment = 0;
        for (size_t i = 0 ; i < m_segments.size(); i++)
        {
            m_segments[i]->segmentMomentStart = cur_moment - m_segments[i]->moment;
            m_segments[i]->segmentMomentEnd = m_segments[i]->segmentMomentStart - (m_segments[i]->length * m_segments[i]->m);

            cur_moment = m_segments[i]->segmentMomentEnd;
            angleSum += GetSegmentTwistAngles(i);
            angleR += ((m_segments[i]->length * pow(10, 2)) / (ShearModulus * m_segments[i]->momentOfInertia));
        }
        double r = (angleSum / angleR) * (-1);
        cur_moment = r;
        for (size_t i = 0 ; i < m_segments.size(); i++)
        {

            m_segments[i]->segmentMomentStart = cur_moment - m_segments[i]->moment;
            m_segments[i]->segmentMomentEnd = m_segments[i]->segmentMomentStart - (m_segments[i]->length * m_segments[i]->m);
            cur_moment = m_segments[i]->segmentMomentEnd;

            GetSegmentTwistAngles(i);
        }
    }
}

void BeamAnalyzer::Build(int condition)
{
    if (condition !=1 && condition !=2 && condition !=3)
    {
        //qWarning
    }
    else if(condition == 1)
    {
        for (size_t i = 0; i < m_segments.size(); i++)
        {
            m_segments[m_segments.size() - (i+1)]->momentOfInertia = m_segments[i]->GetMomentOfInertia();

            m_segments[m_segments.size() - (i+1)]->momentOfResistance = m_segments[i]->GetMomentOfResistance();

        }
        for (size_t i = 0; i < m_segments.size(); i++)
        {
            std::cout<<m_segments[i]->momentOfInertia<<"\n";
            std::cout<<m_segments[i]->momentOfResistance<<"\n";
        }
        CalculateSegmentMoments(condition);
    }
    else if (condition == 2)
    {
        for (size_t i = 0; i < m_segments.size(); i++)
        {
            m_segments[i]->momentOfInertia = m_segments[i]->GetMomentOfInertia();
            std::cout<<m_segments[i]->GetMomentOfInertia()<<"\n";
            m_segments[i]->momentOfResistance = m_segments[i]->GetMomentOfResistance();
            std::cout<<m_segments[i]->GetMomentOfResistance()<<"\n";
        }
        CalculateSegmentMoments(condition);
    }
    else
    {
        for (size_t i = 0; i < m_segments.size(); i++)
        {
            m_segments[i]->momentOfInertia = m_segments[i]->GetMomentOfInertia();
            std::cout<<m_segments[i]->GetMomentOfInertia()<<"\n";
            m_segments[i]->momentOfResistance = m_segments[i]->GetMomentOfResistance();
            std::cout<<m_segments[i]->GetMomentOfResistance()<<"\n";
        }
        CalculateSegmentMoments(condition);
    }
}
