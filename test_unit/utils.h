     
       TEST(Test_ne_pairs)
    {
        ne_pairs ne;
        ne.push_back(1,2);
        ne.push_back(1,2);
        ne.push_back(1,2);
        CHECK_EQUAL(ne.size(),1);
        ne_pairs ne2;
        ne2.push_back(1,3);
        ne2.push_back(ne);
        ne.take();
        CHECK_EQUAL(ne.size(),0);
        CHECK_EQUAL(ne2.size(),2);
        CHECK_EQUAL((ne2.get(0))[0],1);
        CHECK_EQUAL((ne2.get(0))[1],3);
        CHECK_EQUAL((ne2.get(1))[0],1);
        CHECK_EQUAL((ne2.get(1))[1],2);
        ne_pairs ne0;
        ne0.push_back(1,2);
        ne0.push_back(1,3);
        ne0.push_back(1,4);
        ne0.push_back(1,5);
        ne0.push_back(1,6);
        ne0.push_back(1,7);
        ne0.push_back(1,8);
        ne0.push_back(1,9);
        ne2.push_back(ne0);
        CHECK_EQUAL(ne2.size(),8);
    }