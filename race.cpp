#include "race.h"

int RaceRank[StyleCount][MAX_RACE_SEGMENT] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,},
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,},
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,},
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,},
};

void SortVecId(vecId output, StateGrid* state, bool smallFirst = true)
{
    // buble sort
    for(iterVecId iter1 = output.begin();
        iter1 != output.end(); iter1++)
    {
        for(iterVecId iter2 = iter1+1;
            iter2 != output.end(); iter2++)
        {
            const Horse& horse1 = state->GetHorse(*iter1);
            const Horse& horse2 = state->GetHorse(*iter2);

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

int32_t Horse::Collide()
{
    for(uint32_t i=0; i<m_state->GetGridSize(); i++)
    {
        if(i == m_id)
            continue;
        const Horse& tmpHorse = m_state->GetHorse(i);
        if(Overlap(tmpHorse))
            return i;
    }
    return -1;
}

void Horse::SameLine(vecId output)
{
    output.clear();

    for(uint32_t i=0; i<m_state->GetGridSize(); i++)
    {
        if(i == m_id)
            continue;
        if(m_state->GetHorse(i).m_y == m_y)
            output.push_back(i);
    }
}


StateGrid::StateGrid(uint32_t grid_size, const uint32_t *final_rank)
{
    m_crtSegment = 0;
    m_gridSize = grid_size;
    m_finalRank = final_rank;

    for(int i=0; i<m_gridSize; i++)
        m_horses[i].Init(i, m_gridSize, this);
}

StateGrid::~StateGrid()
{
}

bool StateGrid::ShiftRight(uint32_t id)
{
    vecId sameLineHorses;
    m_horses[id].SameLine(sameLineHorses);
    SortVecId(sameLineHorses, this, true);

    int32_t shiftStartId = -1, shiftEndId = -1;

    for(iterVecId iter = sameLineHorses.begin();
        iter != sameLineHorses.end(); iter++)
    {
        Horse& tmpHorse = m_horses[*iter];
        if(tmpHorse.m_x <= m_horses[id].m_x)
            continue;

        if(shiftStartId == -1)
        {
            shiftStartId = *iter;
        }

        // try find a blank while shifting
        if(iter+1 != sameLineHorses.end()){
            if(m_horses[*iter].m_x - m_horses[*(iter+1)].m_x > 1){
                shiftEndId = *iter;
                break;
            }
        }
        else if(shiftEndId == -1)
        {
            // failed to find a blank, shifting all horses
            shiftEndId = *iter;
        }
    }

    if(shiftStartId >= shiftEndId)
        return false;

    // shift now
    for(iterVecId iter = sameLineHorses.begin();
        iter != sameLineHorses.end(); iter++)
    {
        if( m_shifted[*iter] )
            return false;

        if( (*iter) < shiftStartId)
            continue;
        if( (*iter) > shiftEndId)
            break;

        m_horses[*iter].m_x++;
        m_shifted[*iter] = true;
    }

    return true;
}

bool StateGrid::ShiftLeft(uint32_t id)
{
    vecId sameLineHorses;
    m_horses[id].SameLine(sameLineHorses);
    SortVecId(sameLineHorses, this, false);

    int32_t shiftStartId = -1, shiftEndId = -1;

    for(iterVecId iter = sameLineHorses.begin();
        iter != sameLineHorses.end(); iter++)
    {
        Horse& tmpHorse = m_horses[*iter];
        if(tmpHorse.m_x >= m_horses[id].m_x)
            continue;

        if(shiftStartId == -1)
        {
            shiftStartId = *iter;
        }

        // try find a blank while shifting
        if(iter-1 != sameLineHorses.begin()){
            if(m_horses[*iter].m_x - m_horses[*(iter-1)].m_x > 1){
                shiftEndId = *iter;
                break;
            }
        }
        else if(shiftEndId == -1)
        {
            // failed to find a blank, shifting all horses
            shiftEndId = *iter;
        }
    }

    if(shiftStartId <= shiftEndId)
        return false;

    // shift now
    for(iterVecId iter = sameLineHorses.begin();
        iter != sameLineHorses.end(); iter++)
    {
        if( m_shifted[*iter] )
            return false;

        if( (*iter) > shiftStartId)
            continue;
        if( (*iter) < shiftEndId)
            break;

        m_horses[*iter].m_x--;
        m_shifted[*iter] = true;
    }
    return true;
}

bool StateGrid::MoveForward(int32_t id)
{
    int32_t collideId = m_horses[id].Collide();
    if(collideId >= 0)
    {
        // try shift right
        if(m_shifted[collideId] || !ShiftRight(collideId))
        {
            // try shift left
            if(!ShiftLeft(collideId))
            {
                // try move forward

            }
        }
    }
}

bool StateGrid::MoveBackward(int32_t id)
{
}

void StateGrid::Step()
{
    for(int i=0; i<m_gridSize; i++)
        m_shifted[i] = false;

    // update y for horses, move them forward or backwards
    for(int i=0; i<m_gridSize; i++)
    {
        // move winners first to avoid some collision
        uint32_t id = m_finalRank[i];

        m_horses[id].Step(m_crtSegment);
        if(m_horses[id].m_y - m_horses[id].m_oldY > 0 &&
           !MoveForward(id)){

            printf("Failed!");
            return;
        }

    }

    // try shift left

    m_crtSegment++;
}

void StateGrid::Dump()
{
}
