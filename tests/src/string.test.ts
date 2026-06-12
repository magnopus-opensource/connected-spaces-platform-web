import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';

/*
 * Tests for the binding of CSP common::String, using internal binding test types.
 */

describe('String bindings', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('String round trip by value', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testString = 'Hello, world, by value!';

    helper.setCspStringByValue(testString);
    const roundTrip: string = helper.getCspStringByValue();

    expect(testString).toBe(roundTrip);
  });

  it('String round trip by const reference', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testString = 'Hello, world, by const reference!';

    helper.setCspStringByConstRef(testString);
    const roundTrip: string = helper.getCspStringByConstRef();

    expect(testString).toBe(roundTrip);
  });

  it('String round trip by value and const reference are equal', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testString = 'Hello, world, by const reference!';

    helper.setCspStringByValue(testString);
    const roundTripByValue: string = helper.getCspStringByValue();

    helper.setCspStringByConstRef(testString);
    const roundTripByConstRef: string = helper.getCspStringByConstRef();

    expect(roundTripByValue).toBe(roundTripByConstRef);
  });

  it('String replacement returns updated value', () => {
    using helper = csp.BindingsMechanismsTestType.create();

    const testString1 = 'Hello, world!';
    helper.setCspStringByConstRef(testString1);
    const roundTrip: string = helper.getCspStringByConstRef();

    const testStringUpdated2 = 'Hello, universe!';
    helper.setCspStringByConstRef(testStringUpdated2);
    const roundTripUpdated: string = helper.getCspStringByConstRef();

    expect(testStringUpdated2).toBe(roundTripUpdated);
    expect(roundTrip).not.toBe(roundTripUpdated);
  });

  it('String with unicode round trip', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testString = 'впечатляващо! すばらしい! 🚀';

    helper.setCspStringByValue(testString);
    const roundTrip: string = helper.getCspStringByValue();

    helper.setCspStringByConstRef(testString);
    const roundTripConstRef: string = helper.getCspStringByConstRef();

    expect(testString.length).toBe(roundTrip.length);
    expect(testString).toBe(roundTrip);

    expect(testString.length).toBe(roundTripConstRef.length);
    expect(testString).toBe(roundTripConstRef);
  });

  it('String with null character round trip', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testString = 'This should not be \0 truncated!';

    helper.setCspStringByValue(testString);
    const roundTrip: string = helper.getCspStringByValue();

    expect(testString.length).toBe(roundTrip.length);
    expect(testString).toBe(roundTrip);
  });

  it('String with only null characters round trip', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testString = '\0'.repeat(10);

    helper.setCspStringByValue(testString);
    const roundTrip: string = helper.getCspStringByValue();

    expect(testString.length).toBe(roundTrip.length);
    expect(testString).toBe(roundTrip);
  });

  it('Empty string round trip by value', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testString = '';

    helper.setCspStringByValue(testString);
    const roundTrip: string = helper.getCspStringByValue();

    expect(testString.length).toBe(roundTrip.length);
    expect(testString).toBe(roundTrip);
  });

  it('String undefined round trip by value', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testStringUndefined = 'undefined';

    helper.setCspStringByValue(testStringUndefined);
    const roundTrip: string = helper.getCspStringByValue();

    expect(testStringUndefined.length).toBe(roundTrip.length);
    expect(testStringUndefined).toBe(roundTrip);
  });

  it('Long string round trip', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    // 70KB string - There was a stack size of 64KB in the previous CSP wrapper generator.
    // Ensure that we can handle strings larger than that.
    const testString = 'abcdefghij'.repeat(7 * 1024);

    helper.setCspStringByValue(testString);
    const roundTrip: string = helper.getCspStringByValue();

    expect(testString.length).toBe(roundTrip.length);
    expect(testString).toBe(roundTrip);
  });

  it('Setting null or undefined string throws', () => {
    using helper = csp.BindingsMechanismsTestType.create();

    // @ts-expect-error - Typescript will not allow us to set null but want to see what happens anyways
    expect(() => helper.setCspStringByValue(null)).toThrow('Cannot pass non-string to std::string');
    // @ts-expect-error
    expect(() => helper.setCspStringByValue(null)).toThrow('Cannot pass non-string to std::string');

    // @ts-expect-error - Typescript will not allow us to set undefined but want to see what happens anyways
    expect(() => helper.setCspStringByValue(undefined)).toThrow('Cannot pass non-string to std::string');
    // @ts-expect-error
    expect(() => helper.setCspStringByValue(undefined)).toThrow('Cannot pass non-string to std::string');
  });

  it('Setting non-string values throws', () => {
    using helper = csp.BindingsMechanismsTestType.create();

    // @ts-expect-error - Typescript will not allow us to set non-string values but want to see what happens anyways
    expect(() => helper.setCspStringByValue(123)).toThrow('Cannot pass non-string to std::string');
    // @ts-expect-error
    expect(() => helper.setCspStringByValue(123)).toThrow('Cannot pass non-string to std::string');

    // @ts-expect-error
    expect(() => helper.setCspStringByValue(true)).toThrow('Cannot pass non-string to std::string');
    // @ts-expect-error
    expect(() => helper.setCspStringByValue(true)).toThrow('Cannot pass non-string to std::string');

    // @ts-expect-error
    expect(() => helper.setCspStringByValue({ x: 1 })).toThrow('Cannot pass non-string to std::string');
    // @ts-expect-error
    expect(() => helper.setCspStringByValue({ x: 1 })).toThrow('Cannot pass non-string to std::string');
  });

  // Array of string tests

  it('Array of string round trip by value', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testArray = ['alpha', 'beta', 'gamma'];

    helper.setArrayStringByValue(testArray);
    using roundTrip = helper.getArrayStringByValue();

    expect(csp.arrayEquals(roundTrip, testArray)).toBe(true);
  });

  it('Array of string round trip by const reference', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testArray = ['hello', 'world'];

    helper.setArrayStringByConstRef(testArray);
    using roundTrip = helper.getArrayStringByConstRef();

    expect(csp.arrayEquals(roundTrip, testArray)).toBe(true);
  });

  it('Array of string empty array round trip', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testArray: string[] = [];

    helper.setArrayStringByValue(testArray);
    using roundTrip = helper.getArrayStringByValue();

    expect(csp.arrayEquals(roundTrip, testArray)).toBe(true);
  });

  // Map with string tests

  it('Map with string keys round trip by value', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testMap = new Map<string, number>([
      ['key1', 1],
      ['key2', 2]
    ]);

    helper.setMapStringIntByValue(testMap);
    using roundTrip = helper.getMapStringIntByValue();

    expect(csp.mapEquals(roundTrip, testMap)).toBe(true);
  });

  it('Map with string keys round trip by const reference', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testMap = new Map<string, number>([
      ['key1', 1],
      ['key2', 2]
    ]);

    helper.setMapStringIntByConstRef(testMap);
    using roundTrip = helper.getMapStringIntByConstRef();

    expect(csp.mapEquals(roundTrip, testMap)).toBe(true);
  });

  it('Map with empty string key round trip', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testMap = new Map<string, number>([
      ['', 1],
      ['key2', 2]
    ]);

    helper.setMapStringIntByValue(testMap);
    using roundTrip = helper.getMapStringIntByValue();

    expect(csp.mapEquals(roundTrip, testMap)).toBe(true);
  });

  it('Map with string values round trip by value', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testMap = new Map<string, string>([
      ['key1', 'value1'],
      ['key2', 'value2']
    ]);

    helper.setMapStringStringByValue(testMap);
    using roundTrip = helper.getMapStringStringByValue();

    expect(csp.mapEquals(roundTrip, testMap)).toBe(true);
  });

  it('Map with string values round trip by const reference', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testMap = new Map<string, string>([
      ['key1', 'value1'],
      ['key2', 'value2']
    ]);

    helper.setMapStringStringByConstRef(testMap);
    using roundTrip = helper.getMapStringStringByConstRef();

    expect(csp.mapEquals(roundTrip, testMap)).toBe(true);
  });

  it('Map with string values round trip by const reference', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const testMap = new Map<string, string>([
      ['key1', 'value1'],
      ['key2', 'value2']
    ]);

    helper.setMapStringStringByConstRef(testMap);
    using roundTrip = helper.getMapStringStringByConstRef();

    expect(csp.mapEquals(roundTrip, testMap)).toBe(true);
  });
});
