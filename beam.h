#ifndef BEAM_H
#define BEAM_H

#endif // BEAM_H

#include <cstdint>
#include <memory>
#include <vector>
#include "cmath"
//#include <QDebug>

/**
 * Supported beam shape segment types
 */

//Приступить к cpp файлу, вычисление моментов в векторе происходит, как и вычисление углов. Рассомтреть возможность переноса этих вычислений в хеадер. Сделать цпп завтра!!!

enum class SegmentShape
{
    Circle,
    Rectangle,
    Pipe
};

/**
 * The BeamAnalyzer class models a tool for analyzing the behavior of a beam structure
 *
 * Allows you to add and remove various segments with specified dimensions and forces.
 * It calculates and provides information on longitudinal forces, strains, and extensions for each segment
 */

class BeamAnalyzer
{
public:
    /**
  * Constructor.
  */
    explicit BeamAnalyzer();
    /**
      * Adds a round segment to the beam with the specified radius, length, m and moment.
      *
      * @param length is the length of the round segment (in meters)
      * @param size is the diameter of the round segment (in centimeters)
      * @param moment is the moment applied to the round segment (in Hewton meters(N*m))
      * @param m is the ???(честно говоря, не понимаю, что это) applied to the round segment (in Hewtons)
      */
    void AddRoundSegment( double length, double size, double moment, double m );


    /**
     * Adds a rectangular segment to the beam with the specified width, length, formSech, m and moment.
     * @param length the length of the rectangular segment (in meters)
     * @param size the width of the rectangular(had to have a square as a base) segment (in centimeters)
     * @param moment is the moment applied to the rectangular  segment (in Hewton meters(N*m))
     * @param m is the ???(честно говоря, не понимаю, что это) applied to the rectangular segment (in Hewtons)
     * @param formSech(форма сечения) is the proportional form of the rectangular segment h/b(coefficient)
     */
    void AddRectangularSegment( double length, double size, double moment, double m, double formSech );


    /**
     * Adds a pipe segment to the beam with the specified diameter, length, moment, m, and formSech.
     *
     * @param length the length of the pipe-shaped segment (in meters)
     * @param size the diameter of the pipe-shaped segment (in centimeters)
     * @param moment is the moment applied to the pipe-shaped  segment (in Hewton meters(N*m))
     * @param m is the ???(честно говоря, не понимаю, что это) applied to the pipe-shaped segment (in Hewtons)
     * @param formSech(форма сечения) is the proportional form of the pipe-shaped segment d/D(coefficient)
     */
    void AddPipeSegment( double length, double size, double moment, double m, double formSech );


    /**
     * Deletes the segment at the specified index from the beam.
     *
     * @param index the index of the segment to be deleted
     */
    void DeleteSegment( size_t index );


    /**
     * Sets the moment applied at the end of the beam.
     *
     * @param moment is the moment to be applied at the end of the beam
     */
    void SetMomentAtTheEnd( double moment );


    /**
     * Sets the shear modulus(модуль сдвига) applied at the end of the beam.
     *
     * @param pressure is the pressure to be applied at the end of the beam
     */
    void SetShearModulus( double pressure );

    /**
     * Sets the condition applied to the beam.
     *
     * @param condition is the condition of positioning of the beam(Заделка в начале => 1, заделка в конце =>2, заделка и в начале и в конце=>3)
     */
    void SetCondition( int condition );

    /**
     * Returns the moment at the end of the beam.
     */
    double GetMomentAtTheEnd() const;

    /**
     * Returns the shear modulus of the beam.
     */
    double GetShearModulus() const;

    /**
     * Returns the condition of the positioning of the beam.
     */
    int GetCondition() const;


    /**
     * Initiates the analysis of the beam, calculating moments of inertia, moments of resistance, moment, twist angles.
     * @param condition is the condition of positioning of the beam(Заделка в начале => 1, заделка в конце =>2, заделка и в начале и в конце=>3)
     */
    void Build(int condition);


    /**
     * Returns the number of beam segments.
     */
    size_t GetSegmentCount() const;



    /**
     * Returns the moment of inertia force of a segment by index.
     *
     * @param segmentIndex the beam segment index
     */
    double GetSegmentMomentOfInertia( size_t segmentIndex ) const;


    /**
      * Returns the moment of resistance of a segment by index.
      *
      * @param segmentIndex the beam segment index
      */
    double GetSegmentMomentOfResistance( size_t segmentIndex ) const;


    /**
     * Returns the segment moment of a segment by index.
     *
     * @param segmentIndex the beam segment index
     */
    double GetSegmentMoment( size_t segmentIndex) const;





    /**
     * Returns the twist angles of a segment by index.
     *
     * @param segmentIndex the beam segment index
     */
    double GetSegmentTwistAngles( size_t segmentIndex ) const;



    /**
     * Destructor.
     */
    ~BeamAnalyzer();

private:
    class Segment
    {
    public:
        Segment( double lenght, double moment, double m )
            : length( lenght ), moment( moment ), m (m) {}
        double length;
        double moment;
        double m;
        double momentOfInertia;
        double momentOfResistance;
        double segmentMomentStart;
        double segmentMomentEnd;
        double twistAngle;
        virtual double GetMomentOfInertia() const = 0;
        virtual double GetMomentOfResistance() const = 0;
        virtual SegmentShape GetShape() const = 0;

    };
    class RoundSegment : public Segment
    {
    public:
        RoundSegment( double length, double size, double moment, double m )
            : Segment( length, moment, m ), diameter( size )  {}
        double diameter;
        double GetMomentOfInertia() const override
        {
            return (M_PI * pow(diameter, 4) / 32);
        }
        double GetMomentOfResistance() const override
        {
            return (M_PI * pow(diameter, 3) / 16);
        }
        SegmentShape GetShape() const override
        {
            return SegmentShape::Circle;
        }
    };

    class RectangularSegment : public Segment
    {
    public:
        RectangularSegment( double length, double size, double moment, double m, double formSech )
            : Segment( length, moment, m ), width( size ), formSech( formSech ) {}
        double width;
        double formSech;
        double angles[13][3]{ {1, 0.208, 0.140}, {1.2, 0.219, 0.166}, {1.4, 0.228, 0.187},
                              {1.6, 0.234, 0.204}, {1.8, 0.240, 0.217}, {2, 0.246, 0.229},
                              {2.5, 0.258, 0.249}, {3, 0.267, 0.263}, {4, 0.282, 0.281},
                              {6, 0.299, 0.299}, {8, 0.307, 0.307}, {10, 0.313, 0.313}, {11, 0.333, 0.333} };
        double GetMomentOfInertia() const override
        {
            double beta;
            for (int i = 0; i<13; i++)
            {
                if(angles[i][0] == formSech)
                {
                    beta = angles[i][2];
                }
            }
            return (beta * pow(width, 4));
        }
        double GetMomentOfResistance() const override
        {
            double alpha{};
            for (int i = 0; i<13; i++)
            {
                if(angles[i][0] == formSech)
                {
                    alpha = angles[i][1];
                }
            }
            return (alpha * pow(width, 3));
        }
        SegmentShape GetShape() const override
        {
            return SegmentShape::Rectangle;
        }
    };

    class PipeSegment : public Segment
    {
    public:
        PipeSegment( double length, double size, double moment, double m, double formSech )
            : Segment( length, moment, m ), diameter( size ), formSech( formSech ) {}
        double diameter;
        double formSech;
        double GetMomentOfInertia() const override
        {

            return (M_PI * pow(diameter, 4) * (1 - pow(formSech, 4)) / 32);
        }
        double GetMomentOfResistance() const override
        {

            return (M_PI * pow(diameter, 3) * (1 - pow(formSech, 4)) / 16);
        }
        SegmentShape GetShape() const override
        {
            return SegmentShape::Pipe;
        }
    };
private:
    void CalculateSegmentMoments(int condition);
    void AddSegment( std::shared_ptr<Segment> segment );
private:
    double m_momentAtTheEnd;
    double m_shearModulus;
    int m_condition;
    std::vector<std::shared_ptr<Segment>> m_segments;

};
