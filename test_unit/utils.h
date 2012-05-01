     
       TEST(utils,Test_ne_pairs)
    {
        NePairs ne;
        ne.push_back(1,2);
        ne.push_back(1,2);
        ne.push_back(1,2);
        EXPECT_EQ(ne.Size(),1);
        NePairs ne2;
        ne2.push_back(1,3);
        ne2.push_back(ne);
        ne.Take();
        EXPECT_EQ(ne.Size(),0);
        EXPECT_EQ(ne2.Size(),2);
        EXPECT_EQ((ne2.get_pair(0))[0],1);
        EXPECT_EQ((ne2.get_pair(0))[1],3);
        EXPECT_EQ((ne2.get_pair(1))[0],1);
        EXPECT_EQ((ne2.get_pair(1))[1],2);
        NePairs ne0;
        ne0.push_back(1,2);
        ne0.push_back(1,3);
        ne0.push_back(1,4);
        ne0.push_back(1,5);
        ne0.push_back(1,6);
        ne0.push_back(1,7);
        ne0.push_back(1,8);
        ne0.push_back(1,9);
        ne2.push_back(ne0);
        EXPECT_EQ(ne2.Size(),8);
    }
