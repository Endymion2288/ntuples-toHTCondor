/******************************************************************************
 * [Name of the file]
 *      ParticleCand.h
 * [Purpose]
 *      Provide necessary class and functions for resolving multiple candidates
 *      issue in particle reconstruction.
 * [Author]
 *      Eric Wang
 * [Date]
 *      20241211
 * [Note]
 * 
******************************************************************************/
#ifndef ParticleCand_h
#define ParticleCand_h 

#include <vector>
#include <utility>
#include <memory>

class ParticleCand{
public:
    // To mark the type of particle
    enum class PartType{Muon, Track, Jpsi, Upsilon};

    // Name of the particles stored as const std::string
    static const std::string PART_NAME[4];

    using PartIdxList_t  = std::vector<unsigned int>;       // Does not guarantee order
    using PartIdxSet_ptr = std::shared_ptr<PartIdxList_t>;  // Sorted on ascending order.

    // Constructor
    ParticleCand() = default;
    ParticleCand(const ParticleCand& Source) = default;

    // Destructor
    ~ParticleCand() = default;

    // Copy assignment operator
    ParticleCand& operator=(const ParticleCand& Source);

    // Add a certain type of particle to the candidate
    void AddParticle(const PartType&      arg_Type,
                     const PartIdxList_t& arg_IdxList);

    // Compare if there are same particles in two candidates
    bool Overlap(const ParticleCand& arg_Cand) const;
    static bool Overlap(const ParticleCand& arg_Cand1,
                        const ParticleCand& arg_Cand2);

    // Compare if there are same particle indices in two particle sets
    static bool Overlap(const PartIdxSet_ptr& arg_Set1,
                        const PartIdxSet_ptr& arg_Set2);

    // Set the score of the candidate
    void SetCutResult(const bool& arg_PassCut);
    void SetScore(const double& arg_Score);
    void SetId(const unsigned int& arg_Id);

    // Getters
    bool       PassCut() const;
    double    GetScore() const;
    unsigned int GetId() const;
    PartIdxSet_ptr GetParticleIdx(const PartType& arg_Type) const;
    unsigned int   GetParticleIdx(const PartType& arg_Type,
                                  const unsigned int& arg_Index) const;

    // Print out the candidate as human-readable format
    std::string ToString();

    // Clearing the candidate
    void Clear();

private:
    // Member variables
    bool                        m_PassCut;
    double                      m_Score;
    unsigned int                m_Id;
    std::vector<PartType>       m_TypeList;
    std::vector<PartIdxSet_ptr> m_IdxList;
};

#endif