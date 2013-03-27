#include "race.h"
#include <vector>

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
    SortVecId(sameLineHorses, true);

    int32_t shiftStartId = -1, shiftEndId = -1;

    for(iterVecId iter = sameLineHorses.begin();
        iter != sameLineHorses.end(); iter++)
    {
        Horse& tmpHorse = m_horses[*iter];
        if(tmpHorse->m_x <= m_horses[id].m_x)
            continue;

        if(shiftStartId == -1)
        {
            siftStartId = *iter;
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
        if( *m_shifted[*iter] )
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
    SortVecId(sameLineHorses, false);

    int32_t shiftStartId = -1, shiftEndId = -1;

    for(iterVecId iter = sameLineHorses.begin();
        iter != sameLineHorses.end(); iter++)
    {
        Horse& tmpHorse = m_horses[*iter];
        if(tmpHorse->m_x >= m_horses[id].m_x)
            continue;

        if(shiftStartId == -1)
        {
            siftStartId = *iter;
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
        if( *m_shifted[*iter] )
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

bool StateGrid::MoveForward(uint32_t id)
{
    int32_t collideId = m_horses[id].collided();
    if(collideId >= 0)
    {
        // try shift right
        if(m_shifted[collidId] || !ShiftRight(collideId))
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
        if(m_horses[id].m_y - m_hoses[id].m_oldY > 0 &&
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
