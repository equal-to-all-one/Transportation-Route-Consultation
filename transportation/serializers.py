from rest_framework import serializers
from rest_framework_bulk import BulkListSerializer, BulkSerializerMixin
from .models import *
from django.db.models import Q


class DistanceSerializer(serializers.ModelSerializer, BulkSerializerMixin):
    cityA_name = serializers.SerializerMethodField()
    cityB_name = serializers.SerializerMethodField()
    class Meta:
        model = Distance
        fields = '__all__'
        list_serializer_class = BulkListSerializer

    def get_cityA_name(self,obj):
        return obj.cityA.name
    def get_cityB_name(self,obj):
        return obj.cityB.name

    def validate(self, data):
        citya = data['cityA']
        cityb = data['cityB']
        distance = data['distance']
        if distance < 0:
            raise serializers.ValidationError('distance must be greater than 0')
        if citya == cityb and distance > 0:
            raise serializers.ValidationError('you know this is 0 and it has been set by default')
        if self.context['request'].method != 'POST':
            routes = FlightInfo.objects.filter \
                (Q(departure_city=citya, destination_city=cityb)|Q(departure_city=cityb, destination_city=citya)) \
                .values('mileage')
            for route in routes:
                if route < distance:
                    raise serializers.ValidationError('existing transport mileage is lower than new distance')
        return data


class CitySerializer(serializers.ModelSerializer):
    class Meta:
        model = City
        fields = '__all__'

    def create(self, validated_data):
        instance = City.objects.create(**validated_data)
        Distance.objects.create(cityA=instance, cityB=instance, distance=0)
        return instance


class FlightSerializer(serializers.ModelSerializer, BulkSerializerMixin):
    departure_city_name=serializers.SerializerMethodField()
    destination_city_name=serializers.SerializerMethodField()
    time_cost=serializers.SerializerMethodField()
    class Meta:
        model = FlightInfo
        fields = ['id', 'price', 'mileage', 'departure_city_name', 'destination_city_name','departure_city',
                  'destination_city','depart_time', 'arrive_time','time_cost']
        extra_kwargs = {
            'time_cost': {'read_only': True}
        }
        list_serializer_class = BulkListSerializer

    def get_departure_city_name(self,obj):
        return obj.departure_city.name
    def get_destination_city_name(self,obj):
        return obj.destination_city.name

    def get_time_cost(self,obj):
        hours, remainder = divmod(obj.time_cost.seconds, 3600)
        minutes, seconds = divmod(remainder, 60)
        return 'hours: {} -- minutes: {}'.format(int(hours), int(minutes))

    def validate(self, data):
        if data['price'] <= 0:
            raise serializers.ValidationError('are you sure')
        if data['depart_time'] > data['arrive_time']:
            raise serializers.ValidationError('arrive time must be later than depart time')
        if data['departure_city'] == data['destination_city']:
            raise serializers.ValidationError('where do you want to go')
        citya = data['departure_city']
        cityb = data['destination_city']
        obj = Distance.objects.filter(Q(cityA=citya, cityB=cityb)|Q(cityA=cityb, cityB=citya))[0]
        if data['mileage'] < obj.distance:
            raise serializers.ValidationError('transportation mileage can not be shorter than distance: ' + str(obj.distance))
        return data
