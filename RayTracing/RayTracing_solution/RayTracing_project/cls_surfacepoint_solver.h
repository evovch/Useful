#ifndef CLS_SURFACEPOINT_SOLVER_H
#define CLS_SURFACEPOINT_SOLVER_H

#include "cls_vec3.h"

#include "cls_implicit_surface.h"

#include <cstdio>

class cls_surfacepoint_solver
{
public: // internal types

    typedef enum { stat_UNINITIALIZED,
                   stat_INITIALIZED,
                   stat_CONVERGE_SUCCESSFUL,
                   stat_CONVERGE_FAILED
    } status_t;

public: // methods

    cls_surfacepoint_solver() :
        m_max_allowed_iterations(100)
      , m_step_limit(0.001f)
      , m_status(stat_UNINITIALIZED)
      , m_iterations_done(0)
      , m_delta(0.0f)
    {
    }

    ~cls_surfacepoint_solver()
    {
    }

    void Reset(void)
    {
        m_status = stat_UNINITIALIZED;
        m_iterations_done = 0;
        m_delta = 0.0f;
        m_start_point = cls_vec3(0.0f, 0.0f, 0.0f);
        m_current_point = cls_vec3(0.0f, 0.0f, 0.0f);
        m_next_point = cls_vec3(0.0f, 0.0f, 0.0f);
    }

    void SetStartPoint(const cls_vec3& p_q)
    {
        m_status = stat_INITIALIZED;
        m_start_point = p_q;
        m_current_point = p_q;
    }

    void Calculate(cls_implicit_surface const * const p_surface)
    {
        if (m_status != stat_INITIALIZED) {
            //TODO
            return;
        }

        do {

            this->DoIteration(p_surface);

            if (m_delta < m_step_limit) {
                m_status = stat_CONVERGE_SUCCESSFUL;
                return;
            }

            if (m_iterations_done > m_max_allowed_iterations) {
                m_status = stat_CONVERGE_FAILED;
                return;
            }

            m_current_point = m_next_point;

        } while(1);
    }

private: // methods

    void DoIteration(cls_implicit_surface const * const p_surface)
    {
        float f = p_surface->Evalute(m_current_point);
        cls_vec3 f_deriv = p_surface->Evaluate_deriv(m_current_point);
        float tmp = f / (f_deriv*f_deriv);
        m_diff = f_deriv * tmp;
        m_next_point = m_current_point - m_diff;
        m_delta = m_diff.Length();

        fprintf(stderr, "%d: %f, %f, %f -> %f, %f, %f = %f, %f, %f (%f)\n", m_iterations_done,
            m_current_point.x(), m_current_point.y(), m_current_point.z(),
            m_next_point.x(), m_next_point.y(), m_next_point.z(),
            m_diff.x(), m_diff.y(), m_diff.z(),
            m_delta);

        m_iterations_done++;
    }

private: // data members - permanent

    unsigned int m_max_allowed_iterations;

    float m_step_limit;

private: // data members - variable

    status_t m_status;

    unsigned int m_iterations_done;

    float m_delta;

    cls_vec3 m_start_point;

    cls_vec3 m_current_point;

    cls_vec3 m_next_point;

    cls_vec3 m_diff;

};

#endif // CLS_SURFACEPOINT_SOLVER_H
