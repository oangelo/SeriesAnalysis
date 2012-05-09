
TEST(utils,EuclideanDistance){
    std::vector<double> v1,v2;
    v1.push_back(1);
    v1.push_back(1);
    v2.push_back(2);
    v2.push_back(2);
    ASSERT_DOUBLE_EQ(EuclideanDistance(v1,v2),sqrt(2));
}
