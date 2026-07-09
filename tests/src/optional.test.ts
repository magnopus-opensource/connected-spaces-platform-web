import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';

describe('Optional bindings', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  // Basic type tests

  it('Optional round trip basic type by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = 123;

    helper.setOptionalBasicTypeByValue(value);
    const roundTrip = helper.getOptionalBasicTypeByValue();

    expect(roundTrip).toBe(value);
  });

  it('Optional round trip basic type undefined by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();

    helper.setOptionalBasicTypeByValue(undefined);
    const roundTrip = helper.getOptionalBasicTypeByValue();

    expect(roundTrip).toBeUndefined();
  });

  it('Optional basic type equality', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = 123;

    helper.setOptionalBasicTypeByValue(value);
    const roundTrip = helper.getOptionalBasicTypeByValue();

    expect(csp.optionalEquals(roundTrip, value)).toBe(true);
  });

  it('Optional basic type undefined equality', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = undefined;

    helper.setOptionalBasicTypeByValue(value);
    const roundTrip = helper.getOptionalBasicTypeByValue();

    expect(csp.optionalEquals(roundTrip, value)).toBe(true);
  });

  it('Optional round trip basic type by const ref', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = 123;

    helper.setOptionalBasicTypeByConstRef(value);
    const roundTrip = helper.getOptionalBasicTypeByConstRef();

    expect(roundTrip).toBe(value);
  });

  it('Optional round trip basic type undefined by const ref', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();

    helper.setOptionalBasicTypeByConstRef(undefined);
    const roundTrip = helper.getOptionalBasicTypeByConstRef();

    expect(roundTrip).toBeUndefined();
  });

  // Full type tests

  it('Optional round trip full type by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    using elem = csp.BindingsTestType.create(1, 'one');

    helper.setOptionalFullTypeByValue(elem);
    const roundTrip = helper.getOptionalFullTypeByValue();

    expect(csp.optionalEquals(roundTrip, elem)).toBe(true);
  });

  it('Optional round trip full type undefined by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();

    helper.setOptionalFullTypeByValue(undefined);
    const roundTrip = helper.getOptionalFullTypeByValue();

    expect(roundTrip).toBeUndefined();
  });

  it('Optional round trip full type by const ref', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    using elem = csp.BindingsTestType.create(1, 'one');

    helper.setOptionalFullTypeByConstRef(elem);
    const roundTrip = helper.getOptionalFullTypeByConstRef();

    expect(csp.optionalEquals(roundTrip, elem)).toBe(true);
  });

  it('Optional round trip full type undefined by const ref', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();

    helper.setOptionalFullTypeByConstRef(undefined);
    const roundTrip = helper.getOptionalFullTypeByConstRef();

    expect(roundTrip).toBeUndefined();
  });

  it('Optional full type by value disposal', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    using elem = csp.BindingsTestType.create(1, 'one');

    helper.setOptionalFullTypeByValue(elem);
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    {
      using _roundTrip = helper.getOptionalFullTypeByValue();

      const countDuringAlive = csp.BindingsTestType.aliveCount;
      expect(countDuringAlive).toBe(beforeAliveCount + 1);
    }

    const afterAliveCount = csp.BindingsTestType.aliveCount;
    expect(afterAliveCount).toBe(beforeAliveCount);
  });

  it('Optional full type by const ref disposal', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    using elem = csp.BindingsTestType.create(1, 'one');

    helper.setOptionalFullTypeByConstRef(elem);
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    {
      using _roundTrip = helper.getOptionalFullTypeByConstRef();

      const countDuringAlive = csp.BindingsTestType.aliveCount;
      expect(countDuringAlive).toBe(beforeAliveCount + 1);
    }

    const afterAliveCount = csp.BindingsTestType.aliveCount;
    expect(afterAliveCount).toBe(beforeAliveCount);
  });

  it('Optional replacement disposal', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(1, 'two');

    helper.setOptionalFullTypeByValue(elem1);

    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    helper.setOptionalFullTypeByValue(elem2);

    const afterAliveCount = csp.BindingsTestType.aliveCount;
    expect(afterAliveCount).toBe(beforeAliveCount);
  });

  it('Optional mutation does not affect original', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    using elem = csp.BindingsTestType.create(1, 'one');

    helper.setOptionalFullTypeByConstRef(elem);
    const roundTrip = helper.getOptionalFullTypeByConstRef();

    elem.name = 'two';

    expect(csp.optionalEquals(roundTrip, elem)).toBe(false);
  });

  // String tests

  it('Optional round trip string by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = 'hello';

    helper.setOptionalStringByValue(value);
    const roundTrip = helper.getOptionalStringByValue();

    expect(csp.optionalEquals(roundTrip, value)).toBe(true);
  });

  it('Optional round trip string by const ref', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = 'hello';

    helper.setOptionalStringByConstRef(value);
    const roundTrip = helper.getOptionalStringByConstRef();

    expect(csp.optionalEquals(roundTrip, value)).toBe(true);
  });

  // Container tests

  it('Optional round trip container of full types by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const value = [elem1, elem2];

    helper.setOptionalListFullTypeByValue(value);
    const roundTrip = helper.getOptionalListFullTypeByValue();

    expect(csp.optionalEquals(roundTrip, value)).toBe(true);
  });

  it('Optional round trip container of full types by const ref', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const value = [elem1, elem2];

    helper.setOptionalListFullTypeByConstRef(value);
    const roundTrip = helper.getOptionalListFullTypeByConstRef();

    expect(csp.optionalEquals(roundTrip, value)).toBe(true);
  });

  it('Optional undefined container by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = undefined;

    helper.setOptionalListFullTypeByValue(value);

    // using with undefined (or null) is a no-op so no problem in doing this here
    using roundTrip = helper.getOptionalListFullTypeByValue();

    expect(roundTrip).toBe(undefined);
  });

  it('Optional undefined container by const ref', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = undefined;

    helper.setOptionalListFullTypeByConstRef(value);

    // using with undefined (or null) is a no-op so no problem in doing this here
    using roundTrip = helper.getOptionalListFullTypeByConstRef();

    expect(roundTrip).toBe(undefined);
  });

  it('Optional container of full types disposal', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const value = [elem1, elem2];

    helper.setOptionalListFullTypeByConstRef(value);

    const aliveCountBefore = csp.BindingsTestType.aliveCount;

    {
      using _roundTrip = helper.getOptionalListFullTypeByConstRef();

      const aliveCountDuring = csp.BindingsTestType.aliveCount;
      expect(aliveCountDuring).toBe(aliveCountBefore + value.length);
    }

    const aliveCountAfter = csp.BindingsTestType.aliveCount;
    expect(aliveCountAfter).toBe(aliveCountBefore);
  });
});
