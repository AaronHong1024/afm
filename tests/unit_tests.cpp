//
//  unit_tests.cpp
//

#include <iostream>
#include <vector>

#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>

#include <spdlog/spdlog.h>



struct listener : Catch::EventListenerBase
{
    using EventListenerBase::EventListenerBase;
    
    virtual void testCaseStarting(Catch::TestCaseInfo const& testInfo) override
    {
        std::cout << testInfo.tagsAsString() << " " << testInfo.name << std::endl;
    }
};
CATCH_REGISTER_LISTENER(listener)

//------------------------------------------------------------------------------

std::string testfiles_dir = "../tests/files";

//------------------------------------------------------------------------------
#include <parser.hpp>
std::vector<vcfbwt::char_type> random_string(std::size_t length)
{
    //  see: https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
    auto randchar = []() -> char
    {
        const char charset[] = "ATCG";
        const std::size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::vector<vcfbwt::char_type> str(length,0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

TEST_CASE( "Test Parser", "[small]" )
{
    std::size_t random_string_length = 10000;
    
    vcfbwt::pfp::Params params;
    params.w = 3; params.p = 10;
    vcfbwt::pfp::Dictionary<vcfbwt::char_type> dict;
    
    // Parse a random string
    std::vector<vcfbwt::char_type> rs = random_string(random_string_length);
    std::vector<vcfbwt::hash_type> parse;
    std::vector<vcfbwt::char_type> phrase;
    vcfbwt::Mersenne_KarpRabinHash kr_hash(params.w);
    phrase.emplace_back(vcfbwt::pfp::DOLLAR);
    
    for (auto& c : rs)
    {
        phrase.push_back(c);
        if (phrase.size() == params.w) { kr_hash.initialize(phrase.data(), params.w); }
        else if (phrase.size() > params.w) { kr_hash.update(phrase[phrase.size() - params.w - 1], phrase[phrase.size() - 1]); }
        
        if ((phrase.size() > params.w) and ((kr_hash.get_hash() % params.p) == 0))
        {
            vcfbwt::hash_type hash = dict.check_and_add(phrase);
            parse.emplace_back(hash);
            phrase.erase(phrase.begin(), phrase.end() - params.w); // Keep the last w chars
            kr_hash.reset(); kr_hash.initialize(phrase.data(), params.w);
        }
    }
    
    if (phrase.size() >= params.w)
    {
        phrase.insert(phrase.end(), params.w, vcfbwt::pfp::DOLLAR); // Append w dollar at the end
        vcfbwt::hash_type hash = dict.check_and_add(phrase);
        parse.emplace_back(hash);
    }
    
    // Re-parse the string using StringParser
    afm::StringParser sp(params, dict);
    std::vector<std::pair<uint32_t, std::size_t>> parse_sp;
    sp(rs, parse_sp);
    
    bool all_good = true;
    for (std::size_t i = 0; i < parse_sp.size(); i++)
    {
        vcfbwt::size_type rank = parse_sp[i].first;
        std::size_t length = parse_sp[i].second;
        
        if (rank != 0)
        { all_good = all_good and (rank == dict.hash_to_rank(parse[i])); }
    }
    
    REQUIRE(all_good);
}

//------------------------------------------------------------------------------
#include <fmi.hpp>
#include "afm_algorithm.hpp"

TEST_CASE("Test FM_Index", "[small]")
{
    std::vector<vcfbwt::char_type> s = {'A', 'C', 'G', 'T', 'T', 'C', 'C', 'A', 'A', 'C', 'G', 'T', 'A', 'T', 'A', 'C', 'G', 'G', 'G', 'T', '\0'};
    afm::fmi<vcfbwt::char_type, sdsl::wt_rlmn<>, sdsl::csa_wt<>> fmi(s);


//    std::vector<vcfbwt::char_type> p_1 = {'G', 'T'};      REQUIRE(fmi.count(p_1) == 3);
//    std::vector<vcfbwt::char_type> p_2 = {'G'};           REQUIRE(fmi.count(p_2) == 5);
//    std::vector<vcfbwt::char_type> p_3 = {'G', 'T', 'T'}; REQUIRE(fmi.count(p_3) == 1);
//    std::vector<vcfbwt::char_type> p_4 = {'G', 'C'};      REQUIRE(fmi.count(p_4) == 0);
}

TEST_CASE("Test FM_Index integers", "[small]")
{
    std::vector<vcfbwt::size_type> s = {1, 5, 2, 5, 4, 3, 4, 2, 6, 5, 2, 3, 3, 4, 3, 4, 4, 0};
    afm::fmi<vcfbwt::size_type, afm::pfp_wt_sdsl, sdsl::int_vector<>> fmi(s);

    std::vector<vcfbwt::size_type> p_1 = {5, 2};    REQUIRE(fmi.count(p_1) == 2);
    std::vector<vcfbwt::size_type> p_2 = {5};       REQUIRE(fmi.count(p_2) == 3);
    std::vector<vcfbwt::size_type> p_3 = {5, 2, 5}; REQUIRE(fmi.count(p_3) == 1);
    std::vector<vcfbwt::size_type> p_4 = {5, 3};    REQUIRE(fmi.count(p_4) == 0);
}

#include "afm_algorithm.hpp"
TEST_CASE("Test AFM", "[small]")
{
    vcfbwt::pfp::Params params;
    params.w = 4; params.p = 10;
    vcfbwt::pfp::Dictionary<vcfbwt::char_type> dictionary;
    std::vector<vcfbwt::size_type> parse = {1, 5, 3, 2, 4};
    std::vector<std::vector<vcfbwt::char_type>> dictionary_vector = {{'\x02', 'A', 'C', 'G', 'T', 'T', 'C'},
                                                                     {'A', 'A', 'C', 'G', 'T', 'A', 'T'},
                                                                     {'C', 'C', 'A', 'A', 'C', 'G'},
                                                                     {'G', 'T', 'A', 'T', 'A', 'C', 'G', 'G', 'G', 'T', '\n', '\x02', '\x02', '\x02', '\x02'},
                                                                     {'G', 'T', 'T', 'C', 'C', 'A', 'A'}};
    for (auto& phrase : dictionary_vector) { dictionary.check_and_add(phrase); }
    afm::accelerated_fmi afmi;
    afmi.construct(params, dictionary, parse);
    REQUIRE(afmi.count({'G', 'T'}) == 3);
}
//------------------------------------------------------------------------------


int main( int argc, char* argv[] )
{
    Catch::Session session;
    
    using namespace Catch::Clara;
    
    auto cli = session.cli() |
    Opt( testfiles_dir, "dir" ) ["--test-dir"] ("specify the directory containing the test dna sequences files");
    
    session.cli(cli);
    
    int returnCode = session.applyCommandLine(argc, argv);
    
    if( returnCode != 0 ) return returnCode;
    
    session.run();
}