#ifndef __RACE_H__
#define __RACE_H__

#include <stdint.h>

#define MAX_STATE_GRID_SIZE 20
#define MAX_RACE_SEGMENT 10

enum RaceStyle
{
    Linear = 0,
    BlackHorse,
    WhiteHorse,
    Steady,
    StyleCount,
};

int RaceRank[StyleCount][MAX_RACE_SEGMENT] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,},
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,},
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,},
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,},
};

typedef std::pair<uint32_t, uint32_t> idPair;
typedef std::vector<idPair> vecId2;
typedef vecId2::iterator iterVecId2;
typedef std::vector<int> vecId;
typedef vecId::iterator iterVecId;

struct Horse
{
private:
    RaceStyle m_style;
    uint32_t m_raceSegments;
    StateGrid *m_state;
    uint32_t m_id;
public:
    uint32_t m_x, m_y;
    uint32_t m_oldX, m_oldY;
public:
    Horse()
    {
    }

    Init(id, uint32_t raceSegments, StateGrid *state)
    {
        m_raceSegments = raceSegments;
        m_state = state;
    };

    int GetRank(uint32_t segment)
    {
        uint32_t index = ceil(segment / (float) m_raceSegments
                              * MAX_RACE_SEGMENT);

        return RaceRank[m_style][index];
    };

    void Step(uint32_t segment)
    {
        m_oldX = m_x;
        m_oldY = m_y;
        m_y = m_expY;
    };

    int32_t Collide()
    {
        for(uint32_t i=0; i<state->GetGridSize(); i++)
        {
            if(i == m_id)
                continue;
            const Horse& tmpHorse = state->GetHorse(i);
            if(OverLap(tmpHorse))
                return i;
        }
        return -1;
    }

    bool Overlap(const Horse& horse)
    {
        return m_x == horse.m_x &&
            m_y == horse.m_y;
    }

    bool Moveable()
    {
        if(abs(m_x - m_oldX) > 1 ||
           abs(m_y - m_oldY) > 1)
            return false;
        return true;
    }

    void Distace(const Horse& horse, uint32_t *dX, uint32_t *dY)
    {
        dX = horse.m_x - m_x;
        dY = horse.m_y - m_y;
    }

    void SameLine(vecId output)
    {
        output.clear();

        for(uint32_t i=0; i<m_state->GetGridSize(); i++)
        {
            if(i == m_id)
                continue;
            if(state->GetHorse(i).m_y == m_y)
                output.push_back(i);
        }
    }
};

void SortVecId(vecId output, bool smallFirst = true)
{
    // buble sort
    for(iterVecId iter1 = output.begin();
        iter1 != output.end(); iter1++)
    {
        for(iterVecId iter2 = iter1+1;
            iter2 != output.end(); iter2++)
        {
            const Horse& horse1 = m_state->GetHorse(*iter1);
            const Horse& horse2 = m_state->GetHorse(*iter2);

            if( (smallFirst && horse1.m_x > horse2.m_x) ||
                (!smallFirst && horse1.m_x < horse2.m_x) )
            {
                uint32_t tmpId = *iter1;
                *iter1 = *iter2;
                *iter2 = tmpId;
            }
        }
    }
}


class StateGrid
{
private:
    uint32_t m_gridSize;
    Horse m_horses[MAX_STATE_GRID_SIZE];
    bool m_shifted[MAX_STATE_GRID_SIZE];

    uint32_t m_crtSegment;
    const uint32_t* m_finalRank;
protect:
    bool ShiftRight(uint32_t id);
    bool ShiftLeft(uint32_t id);

public:
    StateGrid(const uint32_t grid_size, const uint32_t *final_rank);
    virtual ~StateGrid();

    uint32_t GetGridSize()
    {
        return m_gridSize;
    }

    void Step();
    bool MoveForward(int32_t id);
    bool MoveBackward(int32_t id);

    void Dump();

    Horse& GetHorse(uint32_t id)
    {
        return m_horses[id];
    }
};



#endif /* __RACE_H__ */
