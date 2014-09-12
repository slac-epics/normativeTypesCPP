/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <epicsUnitTest.h>
#include <testMain.h>

#include <pv/nt.h>

using namespace epics::nt;
using namespace epics::pvData;
using std::tr1::dynamic_pointer_cast;
static FieldCreatePtr fieldCreate = getFieldCreate();

void test_builder()
{
    testDiag("test_builder");

    NTNDArrayBuilderPtr builder = NTNDArray::createBuilder();
    testOk(builder.get() != 0, "Got builder");

    StructureConstPtr structure = builder->
            addDescriptor()->
            addTimeStamp()->          
            addAlarm()->  
            addDisplay()->  
            add("extra1",fieldCreate->createScalar(pvString)) ->
            add("extra2",fieldCreate->createScalarArray(pvString)) ->
            createStructure();
    testOk1(structure.get() != 0);
    if (!structure)
        return;

    testOk1(NTNDArray::is_a(structure));
    testOk1(structure->getID() == NTNDArray::URI);
    testOk1(structure->getField("value").get() != 0);
    testOk1(structure->getField("compressedSize").get() != 0);
    testOk1(structure->getField("uncompressedSize").get() != 0);
    testOk1(structure->getField("codec").get() != 0);
    testOk1(structure->getField("dimension").get() != 0);
    testOk1(structure->getField("uniqueId").get() != 0);
    testOk1(structure->getField("dataTimeStamp").get() != 0);
    testOk1(structure->getField("attribute").get() != 0);
    testOk1(structure->getField("descriptor").get() != 0);
    testOk1(structure->getField("alarm").get() != 0);
    testOk1(structure->getField("timeStamp").get() != 0);
    testOk1(structure->getField("display").get() != 0);
    std::cout << *structure << std::endl;

}


void test_narrow()
{
    testDiag("test_narrow");

    NTNDArrayPtr nullPtr = NTNDArray::narrow(PVStructurePtr());
    testOk(nullPtr.get() == 0, "nullptr narrow");

    nullPtr = NTNDArray::narrow(
                getPVDataCreate()->createPVStructure(
                    NTField::get()->createTimeStamp()
                    )
                );
    testOk(nullPtr.get() == 0, "wrong type narrow");


    NTNDArrayBuilderPtr builder = NTNDArray::createBuilder();
    testOk(builder.get() != 0, "Got builder");

    PVStructurePtr pvStructure = builder->
            createPVStructure();
    testOk1(pvStructure.get() != 0);
    if (!pvStructure)
        return;

    testOk1(NTNDArray::is_compatible(pvStructure)==true);
    NTNDArrayPtr ptr = NTNDArray::narrow(pvStructure);
    testOk(ptr.get() != 0, "narrow OK");

    ptr = NTNDArray::narrow_unsafe(pvStructure);
    testOk(ptr.get() != 0, "narrow_unsafe OK");
}

MAIN(testNTNDArray) {
    testPlan(23);
    test_builder();
    test_narrow();
    return testDone();
}


