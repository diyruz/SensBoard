const {
    fromZigbeeConverters,
    exposes
} = require('zigbee-herdsman-converters');
const {
    postfixWithEndpointName,
    getKey,
    precisionRound
} = require('zigbee-herdsman-converters/lib/utils');

const {
    bind
} = require('zigbee-herdsman-converters/lib/reporting');


const CHANNELS_STARTING_ENDPOINT = 2;
const CHANNELS_COUNT = 3;
const ADC_ROUNDING_PRECISION = 3;



const defaultReportingConfig = {
    minimumReportInterval: 0,
    maximumReportInterval: 5,
    reportableChange: 0,
};

const endpointNames = {
    l1: 2,
    l2: 3,
    l3: 4
};
const adcChannels = ['l1', 'l2'];

const deviceExposes = [
    exposes.presets.device_temperature(),
    exposes.numeric('voltage', exposes.access.STATE).withDescription('Battery voltage').withUnit('mV'),
    exposes.presets.battery(),
    ...Object.keys(endpointNames).map(epName => exposes.numeric('present_value', exposes.access.STATE).withEndpoint(epName).withDescription('Current data in device memory')),
    ...Object.keys(endpointNames).map(epName => exposes.numeric('delta_value', exposes.access.STATE).withEndpoint(epName).withDescription('Last reported value')),
    ...Object.keys(endpointNames).map(epName => exposes.numeric('total_value', exposes.access.STATE).withEndpoint(epName).withDescription('Total value processed by device')),
    ...Object.keys(endpointNames).map(epName => exposes.enum('action', exposes.access.SET, ['clear_counter_stats']).withEndpoint(epName).withDescription("Actions")),
    ...adcChannels.map(epName => exposes.numeric('adc_value', exposes.access.STATE).withEndpoint(epName).withDescription('ADC voltage').withUnit('V')),
    ...Object.keys(endpointNames).map(epName => exposes.presets.contact().withEndpoint(epName)),
    exposes.presets.action(Object.keys(endpointNames).map(epName => `toggle_${epName}`))
];

const adcValueConverter = {
    cluster: 'genAnalogInput',
    type: ['attributeReport', 'readResponse'],
    convert: (model, msg, publish, options, meta) => {
        const adcPropName = postfixWithEndpointName('adc_value', msg, model);
        const {
            presentValue
        } = msg.data;
        return {
            [adcPropName]: precisionRound(presentValue, ADC_ROUNDING_PRECISION)
        }
    },
};

const pulseCounterConverter = {
    cluster: 'genMultistateInput',
    type: ['attributeReport', 'readResponse'],
    convert: (model, msg, publish, options, meta) => {
        const totalPropName = postfixWithEndpointName('total_value', msg, model);
        const presentPropName = postfixWithEndpointName('present_value', msg, model);
        const deltaPropName = postfixWithEndpointName('delta_value', msg, model);
        const {
            state = {}
        } = meta;
        const totalValue = state[totalPropName] || 0;
        const prevValue = state[presentPropName] || 0;

        const {
            presentValue
        } = msg.data;

        const diff = prevValue <= presentValue ? presentValue - prevValue : presentValue;

        return {
            [presentPropName]: presentValue,
            [totalPropName]: totalValue + diff,
            [deltaPropName]: diff
        }

    }
};
const clearPulseCounterConverter = {
    key: ['action'],

    convertSet: async (entity, key, value, meta) => {
        const result = {};
        switch (value) {
            case 'clear_counter_stats':
                await entity.write('genMultistateInput', {
                    presentValue: 0
                });
                const names = ['total_value', 'present_value', 'delta_value']
                const endpointName = getKey(endpointNames, entity.ID);
                result.state = {};
                names.forEach(name => result.state[`${name}_${endpointName}`] = 0);
                break;
            default:
                meta.logger.error('Not implemented' + value);
                break;
        }
        return result;

    },
};
const device = {
    zigbeeModel: ['DIYRuZ_SensBoard'],
    model: 'DIYRuZ_SensBoard',
    vendor: 'DIYRuZ',
    description: '[DIYRuZ_SensBoard](http://xxx.ru)',
    fromZigbee: [
        adcValueConverter,
        pulseCounterConverter,
        fromZigbeeConverters.device_temperature,
        fromZigbeeConverters.battery,
        fromZigbeeConverters.ias_contact_alarm_1,
        fromZigbeeConverters.command_toggle
    ],
    toZigbee: [
        clearPulseCounterConverter
    ],
    meta: {
        configureKey: 15,
        multiEndpoint: true,
        battery: {
            voltageToPercentage: '3V_2500_3200' // FYI: https://github.com/Koenkk/zigbee-herdsman-converters/blob/239822497aae9ab51d22c80206b9a42a4265a94a/lib/utils.js#L118
        }
    },
    configure: async (device, coordinatorEndpoint) => {
        const firstEp = device.getEndpoint(1);
        await bind(firstEp, coordinatorEndpoint, [
            'genDeviceTempCfg', 'genPowerCfg'
        ]);
        await firstEp.configureReporting('genDeviceTempCfg', [{
            ...defaultReportingConfig,
            attribute: 'currentTemperature'
        }]);
        await firstEp.configureReporting('genPowerCfg', [{
            ...defaultReportingConfig,
            attribute: 'batteryVoltage',
        }]);

        for (let i = 0; i < CHANNELS_COUNT; i++) {
            const ep = device.getEndpoint(CHANNELS_STARTING_ENDPOINT + i);
            await bind(ep, coordinatorEndpoint, [
                'genMultistateInput', 'genAnalogInput', 'ssIasZone', 'genOnOff'
            ]);
            await ep.configureReporting('genMultistateInput', [{
                ...defaultReportingConfig,
                attribute: 'presentValue',
            }]);
            await ep.configureReporting('genAnalogInput', [{
                ...defaultReportingConfig,
                attribute: 'presentValue',
            }]);
        }
    },
    exposes: deviceExposes,
    endpoint: (device) => endpointNames
};

module.exports = device;